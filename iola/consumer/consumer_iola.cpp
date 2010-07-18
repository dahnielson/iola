/* -*- Mode: C++ ; c-basic-offset: 8 -*- */

// Iola NLE
// Copyright (c) 2010, Anders Dahnielson
//
// Contact: anders@dahnielson.com
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

// STD
#include <algorithm>
#include <stdlib.h>
#include <cstring>
#include <pthread.h>
#include <sys/time.h>
#include <cerrno>

// RLOG
#include <rlog/rlog.h>

// OPENGL
#include <GL/gl.h>
#include <GL/glx.h>

// LIBAO
#include <ao/ao.h>

// IOLA
#include "consumer_iola.h"

// The idea behind this consumer is that you pass it an xid to be used
// with an OpenGL context to draw frames using OpenGL calls in the
// consumer video thread.

namespace iola
{
namespace consumer
{

typedef struct consumer_iola_s *consumer_iola;

struct consumer_iola_s
{
	struct mlt_consumer_s parent;
	mlt_properties properties;
	pthread_t thread;
	bool joined;
	bool running;
	int window_width;
	int window_height;
	int width;
	int height;
	Display* display;
	int screen;
	XVisualInfo* visual;
	Window xid;
	GLXContext context;
	GLuint texture;
	int audio_driver;
	ao_device* audio_device;
	ao_sample_format audio_format;
	char audio_buffer[4096 * 10];
	int audio_buffer_fill;
	int audio_buffer_period;
};

static int consumer_start(mlt_consumer parent);
static int consumer_stop(mlt_consumer parent);
static int consumer_is_stopped(mlt_consumer parent);
static void consumer_close(mlt_consumer parent);
static void* consumer_thread(void *);

// Initialize a consumer object ("ctor")
void* consumer_iola_init(mlt_profile profile, mlt_service_type type, const char *id, const void *arg)
{
	// Create the consumer object
	consumer_iola self = (consumer_iola_s*) calloc(sizeof(struct consumer_iola_s), 1);

	// If no malloc'd and consumer init ok
	if (self != NULL && mlt_consumer_init(&self->parent, self, profile) == 0)
	{
		rDebug("%s: Constructing iola consumer object", __PRETTY_FUNCTION__);

		// Get the parent consumer object
		mlt_consumer parent = &self->parent;

		// We have stuff to clean up, so override the close method
		parent->close = consumer_close;

		// Get a handle on properties
		mlt_service service = MLT_CONSUMER_SERVICE(parent);
		self->properties = MLT_SERVICE_PROPERTIES(service);

		// Set the default volume
		mlt_properties_set_double(self->properties, "volume", 1.0);

		// Set buffer to one frame for low latency
		mlt_properties_set_int(self->properties, "buffer", 1);

		// Set rescaling
		mlt_properties_set(self->properties, "rescale", "none");

		// Set fit to window
		mlt_properties_set_int(self->properties, "fit_to_window", false);

		// Default audio buffer
		mlt_properties_set_int(self->properties, "audio_buffer", 512);

		// Ensure we don't join on a non-running object
		self->joined = true;

		// Allow thread to be started/stopped
		parent->start = consumer_start;
		parent->stop = consumer_stop;
		parent->is_stopped = consumer_is_stopped;

		// Return the consumer produced
		return parent;
	}
	else
		rError("%s: Construction of iola consumer object failed", __PRETTY_FUNCTION__);

	// malloc or consumer init failed
	free(self);

	// Indicate failure
	return 0;
}

// Consumer start method
int consumer_start(mlt_consumer parent)
{
	// Get the actual object
	consumer_iola self = (consumer_iola_s*) parent->child;

	// If we're not already running
	if (!self->running)
	{
		consumer_stop(parent);

		rDebug("%s: Starting iola consumer", __PRETTY_FUNCTION__);

		// Create the thread
		self->running = true;
		self->joined = false;
		pthread_create(&self->thread, NULL, consumer_thread, self);

		rDebug("%s: Consumer thread created", __PRETTY_FUNCTION__);
	}

	return 0;
}

// Consumer stop method
int consumer_stop(mlt_consumer parent)
{
	// Get the actual object
	consumer_iola self = (consumer_iola_s*) parent->child;

	// If we've not already joined
	if (!self->joined)
	{
		rDebug("%s: Stopping iola consumer", __PRETTY_FUNCTION__);

		// Kill the thread
		self->joined = true;
		self->running = false;
		if (self->thread)
			pthread_join(self->thread, NULL);

		rDebug("%s: Consumer joined main thread", __PRETTY_FUNCTION__);
	}

	return 0;
}

// Consumer is stopped method
int consumer_is_stopped(mlt_consumer parent)
{
	// Get the actual object
	consumer_iola self = (consumer_iola_s*) parent->child;

	return !self->running;
}

// Consumer close method ("dtor")
static void consumer_close(mlt_consumer parent)
{
	// Get the actual object
	consumer_iola self = (consumer_iola_s*) parent->child;

	rDebug("%s: Destroying iola consumer object", __PRETTY_FUNCTION__);

	// Now clean up the rest
	mlt_consumer_close(parent);

	// Finally clean up this
	free(self);
}

// Draw frame on display
static int consumer_play_video(consumer_iola self, mlt_frame frame)
{
	// Get window dimensions
	XWindowAttributes attr;
	XGetWindowAttributes(self->display, self->xid, &attr);
	self->window_width = attr.width;
	self->window_height = attr.height;
	glViewport(0, 0, self->window_width, self->window_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, self->window_width, self->window_height, 0, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// The image
	uint8_t* image;

	// The video format
	mlt_image_format vfmt = mlt_image_rgb24;

	// Width and height
	int width = self->width, height = self->height;

	if (self->running)
	{
		// Get image, format, width and height
		mlt_frame_get_image(frame, &image, &vfmt, &width, &height, 0);
		mlt_properties_set_int(MLT_FRAME_PROPERTIES(frame), "format", vfmt);

		// Check that we got the format we asked for
		if (vfmt != mlt_image_rgb24)
			return 0;

		// Store the width and height received
		self->width = width;
		self->height = height;

		double w = self->width;
		double h = self->height;

		// Fit frame into window
		bool zoom = mlt_properties_get_int(self->properties, "fit_to_window");
		if (zoom)
		{
			double window_aspect = (double)self->window_width / self->window_height;
			double frame_aspect = mlt_frame_get_aspect_ratio(frame) * self->width / self->height;
			w = frame_aspect / window_aspect * self->window_width;
			h = self->window_height;
			if (w > self->window_width)
			{
				w = self->window_width;
				h = window_aspect / frame_aspect * self->window_height;
			}
		}

		// Center frame in window
		int x, y;
		x = (self->window_width - w) / 2;
		y = (self->window_height - h) / 2;
		x -= x % 2;

		// ... load the image ...
		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, self->texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB,
			     self->width, self->height, 0, GL_RGB,
			     GL_UNSIGNED_BYTE, image);

		// ... draw the image ...
		glClear(GL_COLOR_BUFFER_BIT);

		const double u = self->width, v = self->height;

		glEnable(GL_TEXTURE_RECTANGLE_ARB);
		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, self->texture);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0, 0, 0, 1);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(x,   y);
		glTexCoord2f(u, 0); glVertex2f(x+w, y);
		glTexCoord2f(u, v); glVertex2f(x+w, y+h);
		glTexCoord2f(0, v); glVertex2f(x,   y+h);
		glEnd();
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_RECTANGLE_ARB);
	}

	return 0;
}

// Output frame audio
static int consumer_play_audio(consumer_iola self, mlt_frame frame)
{
	// The pcm
	char* pcm;

	// The audio format
	mlt_audio_format afmt = mlt_audio_s16;

	// Frequency, channels and samples
	int frequency = self->audio_format.rate;
	int channels = self->audio_format.channels;
	static int counter = 0;
	int samples = mlt_sample_calculator(mlt_properties_get_double(self->properties, "fps"), frequency, counter++);
	if (self->running)
	{
		// Get pcm, format, frequency, channels and samples
		mlt_frame_get_audio(frame, (void**) &pcm, &afmt, &frequency, &channels, &samples);

		// Check that we got the format we asked for
		if (afmt != mlt_audio_s16)
			return 0;

		// Play samples if buffer is filled
		if (self->audio_buffer_fill + samples >= self->audio_buffer_period)
		{
			ao_play(self->audio_device, self->audio_buffer, self->audio_buffer_fill * channels * 2);
			self->audio_buffer_fill = 0;
		}

		// Copy samples to buffer
		memcpy(self->audio_buffer + self->audio_buffer_fill * channels * 2, pcm, samples * channels * 2);
		self->audio_buffer_fill += samples;
	}

	return 0;
}

// Consumer thread
static void* consumer_thread(void *arg)
{
	rDebug("%s: Entering iola consumer thread", __PRETTY_FUNCTION__);

	// Identify the arg
	consumer_iola self = (consumer_iola_s*) arg;

	// Get the consumer
	mlt_consumer consumer = &self->parent;

	// Convenience functionality
	int terminate_on_pause = mlt_properties_get_int(MLT_CONSUMER_PROPERTIES(consumer), "terminate_on_pause");
	bool terminated = false;

	// Get real time flag
	int real_time = mlt_properties_get_int(self->properties, "real_time");

	// Internal intialization
	mlt_frame frame = NULL;

	// Audio format
	self->audio_format.bits = 16;
	self->audio_format.channels = mlt_properties_get_int(self->properties, "channels");
	self->audio_format.rate = mlt_properties_get_int(self->properties, "frequency");
	self->audio_format.byte_format = AO_FMT_NATIVE;

	// Audio driver
	self->audio_driver = ao_default_driver_id();
	if (self->audio_driver == -1)
		rWarning("%s: No usable audio output device found!", __PRETTY_FUNCTION__);

	// Audio device
	self->audio_device = ao_open_live(self->audio_driver, &self->audio_format, NULL);
	if (!self->audio_device)
	{
		switch (errno)
		{
		case AO_ENODRIVER:
			rError("%s: No audio driver corresponds to driver_id=%i",
			       __PRETTY_FUNCTION__, self->audio_driver);
		case AO_ENOTLIVE:
			rError("%s: The selected audio driver is not a live output device.",
			       __PRETTY_FUNCTION__);
		case AO_EBADOPTION:
			rError("%s: A valid option key to the audio device has an invalid value.",
			       __PRETTY_FUNCTION__);
		case AO_EOPENDEVICE:
			rError("%s: Cannot open the audio device",
			       __PRETTY_FUNCTION__);
		case AO_EFAIL:
			rError("%s: Opening of audio device failed",
			       __PRETTY_FUNCTION__);
		default:
			rError("%s: No audio device", __PRETTY_FUNCTION__);
		};
	}

	// Audio buffer
	self->audio_buffer_fill = 0;
	self->audio_buffer_period = mlt_properties_get_int(self->properties, "audio_buffer");

	if (!self->audio_buffer)
		rError("%s: No audio buffer allocated!", __PRETTY_FUNCTION__);

	// Create OpenGL context
	int attrib[] = {
		GLX_RGBA,
		GLX_RED_SIZE,   8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE,  8,
		GLX_DOUBLEBUFFER,
		0
	};

	self->display = XOpenDisplay(NULL);
	self->screen = DefaultScreen(self->display);
	self->visual = glXChooseVisual(self->display, self->screen, attrib);
	self->context = glXCreateContext(self->display, self->visual, 0, 1);
	self->xid = mlt_properties_get_int(self->properties, "xid");
	glXMakeCurrent(self->display, self->xid, self->context);

	// Set up OpenGL
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Generate a texture name
	glGenTextures(1, &self->texture);

	// Get FPS
	double fps = mlt_properties_get_double(self->properties, "fps");

	// Timer
	struct timeval now;
	int64_t start = 0;
	int64_t delta = 0;
	int64_t accumulator = 0;
	int64_t duration = 1000000 / fps;
	int64_t max_lag = 250000;

	gettimeofday(&now, NULL);
	start = (int64_t)now.tv_sec * 1000000 + now.tv_usec;

	// Loop until told not to
	while(!terminated && self->running)
	{
		gettimeofday(&now, NULL);
		delta = ((int64_t)now.tv_sec * 1000000 + now.tv_usec) - start;
		start = (int64_t)now.tv_sec * 1000000 + now.tv_usec;
		accumulator +=  std::min(delta, max_lag);

		while(accumulator >= duration)
		{
			accumulator -= duration;

			// Close old frame if any
			if (frame != NULL)
				mlt_frame_close(frame);

			// Get a frame from the attached producer
			frame = mlt_consumer_rt_frame(consumer);

			// Output audio
			if (frame != NULL)
				consumer_play_audio(self, frame);
		}

		// Check for termination
		if (terminate_on_pause && frame != NULL)
			terminated = mlt_properties_get_double(MLT_FRAME_PROPERTIES(frame), "_speed") == 0.0;

		// Ensure that we have a frame
		if (frame != NULL)
		{
			// Draw the frame
			consumer_play_video(self, frame);

			// Swap buffers
			glXSwapBuffers(self->display, self->xid);

			// Fire event
			mlt_events_fire(self->properties, "consumer-frame-show", frame, NULL); //NOTE This will cause the thread to call functions (the listeners) in other threads (i.e. the main thread) and block.
		}
	}

	self->running = false;

	ao_close(self->audio_device);

	glDeleteTextures(1, &self->texture);
	glXDestroyContext(self->display, self->context);
	XCloseDisplay(self->display);

	mlt_consumer_stopped(&self->parent);

	rDebug("%s: Exiting iola consumer thread", __PRETTY_FUNCTION__);

	return NULL;
}

} // namespace consumer
} // namespace iola
