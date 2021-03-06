EDL
===

This is a brief description of the EDL file that Iola could possibly
export. It is based on the extended CMX 3600 format supported by many
digital nonlinear editing systems.

**Note:** The EBNF below is incomplete, some basic types are only implied
          and not defined.

**Note:** The DLEDL extension used by Autodesk (former Discreet Logic)
          products is also included.

Structure
---------

    edl = title, {statement}, [source-table], eof;

    title = "TITLE:", sp, string, eol;

    statement = fcm | edit-decision | aud | note | dledl;

    fcm = "FCM:", sp, ("DROP FRAME" | "NON-DROP FRAME"), eol;

    edit-decision = event, sp, reel, sp, channels, sp, operation,
                sp, [operation-duration, sp], source-in-time, sp,
                source-out-time, sp, dest-in-time, sp, dest-out-time,
                eol;

    event = integer;
    reel = reel-black | reel-id;
    reel-black = "BL" | "BLK";
    reel-id = string;
    channels = channel-v | channel-a1 | channel-a2 | channel-a12 |
           channel-a12v | channel-a2v | channel-none; 
    channel-v = "V";
    channel-a1 = "A";
    channel-a2 = "A2;
    channel-a12 = "AA";
    channel-a12v = "AA/V";
    channel-a2v = "A2/V";
    channel-none = "NONE";
    operation = cut | disolve | wipe;
    cut = "C";
    disolve = "D";
    wipe = "W", wipe-smpte;
    wipe-smpte = integer;
    operation-duration = integer;
    source-in-time = fcm-timecode;
    source-out-time = fcm-timecode;
    dest-in-time = fcm-timecode;
    dest-out-time = fcm-timecode;
    fcm-timecode = hours, ":", minutes, ":", seconds, ":", frames;

    aud = "AUD:", sp, {aud-channel, sp}, eol;
    aud-channel = integer;

    note = "*", sp, (note-comment | note-from-clip-name |
           note-from-clip-still | note-fcm | note-effect-name), eol;
    note-comment = "COMMENT:", sp, string;
    note-from-clip-name = "FROM CLIP NAME:", sp, string;
    note-from-clip-still = "FROM CLIP IS A STILL";
    note-fcm = "FCM:", sp, "DIGITAL TV", sp, 
           ("24P" | "25P" | "30P" | "60P"); 
    note-effect-name = "EFFECT NAME:", sp, string;

    dledl = "DLEDL:", sp, (dledl-start-tc | dledl-path | dledl-edit |
            dledl-reel), eol; 
    dledl-start-tc = "START TC:", sp, dledl-timecode;
    dledl-path = "PATH:", sp, string;
    dledl-edit = "EDIT:0 FILENAME:", sp, string;
    dledl-reel = "REEL:", sp, reel-id, sp, reel-id-long;
    dledl-timecode = dledl-timecode-df | dledl-timecode-ndf;
    dledl-timecode-df = hours, ":", minutes, ":", seconds, ":", frames;
    dledl-timecode-ndf = hours, ";", minutes, ";", seconds, ";", frames;

    source-table = {source};
    source = ">>>", sp, "SOURCE", sp, reel-id, sp, reel-id-long, eol;
    reel-id-long = string;
