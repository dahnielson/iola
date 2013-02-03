MLT++ Quick Reference
=====================

Mlt::Producer
-------------

`int seek(int position)`
Seek to a specified position. Parameter position set the "play head"
position of the producer.

`int position()`
Get the current position (relative to in point). Returns the position
of the "play head" relative to its beginning.

`int frame()`
Get the current position (relative to start of producer). Returns the
position of the "play head" regardless of the in point.

`int set_speed(double speed)`
Set the playing speed. Parameter speed 	the new speed as a relative
factor (1.0 = normal).

`double get_speed()`
Get the playing speed. Returns the speed as a relative factor (1.0 =
normal).

`double get_fps()`
Get the frames per second. This is determined by the producer's
profile. Returns the video refresh rate.

`int set_in_and_out(int in, int out)`
Set the in and out points. The in point is where play out should start
relative to the natural start of the underlying file. The out point is
where play out should end, also relative to the start of the
underlying file. If the underlying resource is a live stream, then the
in point is an offset relative to first usable sample. Parameter in
set the relative starting time; a negative value is the same as 0, and
parameter out the relative ending time; a negative value is the same
as length - 1.

`int get_in()`
Get the in point. Returns the in point.

`int get_out()`
Get the out point. Returns the out point.

`int get_length()`
Get the total, unedited length of the producer. The value returned by
a live streaming producer is unknown. Returns the duration of the
producer regardless of in and out points.

`int get_playtime()`
Get the total play time. Returns the playable (based on in and out
points) duration.

`Producer *cut(int in = 0, int out = -1)`
Create a cut of this producer. A "cut" is a portion of another
(parent) producer. Parameters in the beginning and out the
end. Returns the new producer.

`bool is_cut()`
Determine if producer is a cut. Returns true if this is a "cut"
producer.

`bool is_blank()`
Determine if the producer is a blank. Blank producers should only
appear as an item in a playlist. Returns true if this is a "blank"
producer.

`void optimise()`
Optimise for overlapping cuts from the same clip. 

`int clear()`
Physically reduce the producer (typically a cut) to a 0 length.
Essentially, all 0 length cuts should be immediately removed by
containers.


Mlt::Playlist
-------------

`int count()`
Get number of clips in the playlist. Returns the number of playlist
entries.

`int clear()`
Clear the playlist. 

`int remove(int where)`
Remove an entry in the playlist. Parameter where the playlist entry
index.

`int move(int from, int to)`
Move an entry in the playlist. Parameter src an entry index and dest
an entry index.

`int split_at(int position, bool left = true)`
Split the playlist at the absolute position. Parameters position the
time at which to split relative to the beginning of the clip, left
true to split before the frame starting at position.

`int remove_region(int position, int length)`
Remove a portion of the playlist by time. Parameters position the
starting time and length the duration of time to remove. Returns the
new entry index at the position.

`int move_region(int position, int length, int new_position)`
Not implemented.

Clips
-----

`int append(Producer &producer, int in = -1, int out = -1)`
Append a producer to the playlist. Parameters producer the producer to
append, in the starting point on the producer; a negative value is the
same as 0, out the ending point on the producer; a negative value is
the same as producer length - 1.

`int insert(Producer &producer, int where, int in = -1, int out = -1)`
Insert a producer into the playlist. Parameters producer the producer
to insert, where the producer's playlist entry index, in the starting
point on the producer, and out the ending point on the producer.

`int insert_at(int position, Producer *producer, int mode = 0)`
`int insert_at(int position, Producer &producer, int mode = 0)`
Insert a clip at a specific time. Parameters position the time at
which to insert, producer the producer to insert, and mode true if you
want to overwrite any blank section.

`int clip(mlt_whence whence, int index)`
Get the position which corresponds to the start of the next
clip. Parameters whence the location from which to make the index
relative: start of playlist, end of playlist, or current position, and
index the playlist entry index relative to whence. Returns the time at
which the referenced clip starts.

`int current_clip()`
Obtain the current clips index. Returns the index of the playlist
entry at the current position.

`Producer* current()`
Obtain the current clips producer. Returns the producer at the current
position.

`ClipInfo* clip_info(int index, ClipInfo *info = NULL)`
Get all the info about the clip specified. Parameters info a clip info
struct and index a playlist entry index.

`static void delete_clip_info(ClipInfo *info)`
Delete the clip info struct.

`Producer* get_clip(int clip)`
Return the clip at the clip index. Parameter clip the index of a
playlist entry. Returns a producer or NULL if there was an error.

`Producer* get_clip_at(int position)`
Return the clip at the specified position. Parameter position a time
relative to the beginning of the playlist. Returns a producer or NULL
if not found.

`int get_clip_index_at(int position)`
Return the clip index of the specified position. Parameter position a
time relative to the beginning of the playlist.

`int clip_start(int clip)`
Get the time at which the clip starts relative to the
playlist. Parameter clip the index of the playlist entry. Returns the
starting time.

`int clip_length(int clip)`
Get the playable duration of the clip. Parameter clip the index of the
playlist entry. Returns the duration of the playlist entry.

`int resize_clip(int clip, int in, int out)`
Resize the specified clip. Parameters clip the index of the playlist
entry, in the new starting time on the clip's producer; a negative
value is the same as 0, and out	the new ending time on the clip's
producer; a negative value is the same as length - 1.

`int split(int clip, int position)`
Split a clip on the playlist at the given position. This splits after
the specified frame. Parameters clip the index of the playlist entry,
and position the time at which to split relative to the beginning of
the clip or its end if negative.

`int join(int clip, int count = 1, int merge = 1)`
Join 1 or more consecutive clips. Parameters clip the starting
playlist entry index, count the number of entries to merge, and
merge ignored.

`int repeat(int clip, int count)`
Repeat the specified clip n times. Parameters clip a playlist entry
index and count the number of times to repeat the clip.

Blanks
------

`bool is_blank(int clip)`
Determine if the specified clip index is a blank. Parameter clip the
index of the playlist entry. Returns true if there was an error.

`bool is_blank_at(int position)`
Determine if the specified position is a blank. Parameter position a
time relative to the start or end (negative) of the playlist. Returns
true if there was an error.

`void consolidate_blanks(int keep_length = 0)`
Consolidate adjacent blank producers. Parameter keep_length set false
to remove the last entry if it is blank.

`int blank(int length)`
Append a blank to the playlist of a given length. Parameter length the
ending time of the blank entry, not its duration.

`void insert_blank(int clip, int length)`
Insert blank space. Parameters clip the index of the new blank section
and length the ending time of the new blank section (duration - 1).

`Producer* replace_with_blank(int clip)`
Replace the specified clip with a blank and return the clip. Parameter
clip the index of the playlist entry. Returns a producer or NULL if
there was an error.

`void pad_blanks(int position, int length, int find = 0)`
Resize a blank entry. Parameters position the time at which the blank
entry exists relative to the start or end (negative) of the playlist,
length the additional amount of blank frames to add, and find true to
fist locate the blank after the clip at position.

`int blanks_from(int clip, int bounded = 0)`
Get the duration of a blank space. Parameters clip the index of the
playlist entry and bounded the maximum number of blank entries or 0
for all. Returns the duration of a blank section.

Mixes
-----

`bool is_mix(int clip)`
Determine if the clip is a mix. Parameter clip the index of the
playlist entry. Returns true if the producer is a mix.

`int mix(int clip, int length, Transition *transition = NULL)`
Mix consecutive clips for a specified length and apply transition if
specified. Parameters clip	the index of the playlist entry,
length	the number of frames over which to create the mix, and
transition the transition to use for the mix.

`int mix_add(int clip, Transition *transition)`
Add a transition to an existing mix. Parameters clip the index of the
playlist entry and transition a transition.
