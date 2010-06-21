#include <cstdlib>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/x.H>

#include <mlt++/Mlt.h>
using namespace Mlt;

int main(int argc, char **argv)
{
	Factory::init(NULL);
	Profile profile;

	Fl_Window* window = new Fl_Window(profile.width(), profile.height());
	window->end();
	window->show();
	Fl::wait();

	Producer producer(profile, argv[1]);
	Consumer consumer(profile, "sdl");
	consumer.set("rescale", "none");
	consumer.connect(producer);

	char temp[132];
	sprintf(temp, "%d", (int)fl_xid(window));
	setenv("SDL_WINDOWID", temp, 1);
	consumer.start();

	return Fl::run();
}
