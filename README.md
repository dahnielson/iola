Iola NLE
========

Terminology
-----------

The user interface is split up into three panels: source monitor,
program monitor and timeline. There are two types of assets: clips and
sequences. Clips are the source material you will edit and assemble
into a sequence. The source monitor is used to view clips and mark in
and out points in them for editing. The program monitor is used to
view the sequence and edit clips into it. The timeline is used to get
an oveview of the sequence and to edit clips into it.


Editing
-------

### Keyboard shortcuts #

Iola NLE support the most common keyboard shortcuts for editing.

    Q     goto mark in
    W     goto mark out

    E     set mark in
    R     set mark out
    T     mark clip / cut

    I     set mark in
    O     set mark out

    A     goto previous edit
    S     goto next edit

    D     clear mark in
    F     clear mark out
    G     clear mark in and out

    J     play backward
    K     stop
    L     play forward

    Z     lift
    X     extract
    V     insert
    B     overwrite

    Home  goto start
    End   goto end

### Lift #

When you edit a sequence using lift edit, all sequence clips in all
unlocked tracks are removed and is replaced with a black filler to
fill the gap.

### Extract #

When you edit a sequence using extract edit, all sequence clips in all
unlocked tracks are removed and the gap left by the removal is closed.

### Insert #

When you edit a clip into a sequence using insert edit, all sequence
clips in all unlocked tracks are cut at the In point of your edit and
pushed forward in the edited sequence by the duration of your source
clip.

### Overwrite #

When you edit a clip into a sequence using overwrite edit, any
portions of clips that are already in the destination tracks are
replaced by the source clip.


Container & codec
-----------------

Iola NLE use FFMPEG to read and write video files and is only limited
by what FFMPEG can handle. But not all containers and video codecs are
suitable for editing and video production.

### Containers #
          
 - AVI       Audio Video Interleave
 - GFX       General eXchange Format, SMPTE 360M
 - MOV       QuickTime
 - MXF       Material eXchange Format, SMPTE 377M

### Video codecs #

For editing:

 - DNxHD     lossy intraframe high-definition video compression
 - HuffYUV   lossless intraframe compression
 - MJPEG     lossy intraframe compression

For aquiring media:

 - H.264     lossy interframe compression (eg. from DSLR)
 - MPEG-2    lossy interframe compression (eg. from HDV)


Conforming
----------

To conform a sequence with another application it must be exported to
an interchangable format.

 - EDL (CMX 3600)
 - FCP XML (Final Cut Pro XML Interchange Format, version 5)
 - ALE (Avid Log Exchange)
