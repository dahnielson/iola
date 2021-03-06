Video Formats
=============

DAR, SAR and PAR
----------------

*Display Aspect Ratio* (DAR) is the aspect ratio of the image as
displayed.

*Storage Aspect Ratio* (SAR) is the aspect ratio of pixel dimensions. If
an image is displayed with square pixels, then these ratios agree; if
not, then non-square pixels are used, and these ratios disagree.

*Pixel Aspect Ratio* (PAR) is the aspect ratio of the pixels
themselves -- for square pixels this is 1:1.

These are related by the identity:

    SAR = W / H

And can thus be solved:

    DAR = SAR × PAR
    PAR = DAR / SAR

SAR is calculated using the dimension of the stored image in pixels:

    SAR = DAR / PAR


Production and Clean Aperture
-----------------------------

The *production aperture* (PA) is the minimum sub-rectangle of the video
signal that should be used in order to avoid cutting off any of the
data.

The *clean aperture,* (CA) is co-centric with the production aperture and
the picture center, and contained within the production aperture, is a
sub-rectangle of the video that has the system's standard DAR.


Common PARs
-----------

    Description		PAR

    Square			 1:1
    ITU-R Rec.601 (PAL)	59:54 [1]
    ITU-R Rec.601 (NTSC)	10:11 [1]
    HD (1440x1080)		 4:3
    HD (1280x1080)		 3:2
    HD (960x720)		 4:3

[1] <http://lurkertech.com/lg/video-systems/>

Converting Non-Square to Square
-------------------------------

1. Scale resolution horizontally by non-square PAR (e.g. 10/11)
2. Crop resolution to square width (e.g. 640)


Common Resolutions
------------------

    PA	     SAR        DAR     PAR	CA

    720x576	      5:4	4:3	59:54	702+54/59x576 [1] or (768×(54/59))x576 [2]
    720x486	     40:27      4:3	10:11	704x480       [1] or (640×(11/10))x480 [2]

    720x576	      5:4	4:3	59:54	704x576 [1]
    720x480	      3:2	4:3	10:11	704x480 [1]

    768x576	      4:3	4:3	 1:1	768x576 [2]
    640x486	    320:243	4:3	 1:1 	640x480 [2]

    1920x1080    16:9      16:9	 1:1	1888x1062 [2]
    1440x1080     4:3      16:9	 4:3
    1280x1080    32:27     16:9	 3:2
    1280x720     16:9      16:9	 1:1	1248x702 [2]
    960x720	      4:3      16:9	 4:3

    4096x2304    16:9      16:9	 1:1
    2048x1152    16:9      16:9	 1:1

**Note:** The SD resolutions also have anamorphic versions in which the
          PAR and DAR is mulitplied by 4:3.

The stated DAR is in relation to the CA.

[1] <http://www.mir.com/DMG/aspect.html>
[2] <http://lurkertech.com/lg/video-systems/>


Production Format Matrix
------------------------

    Description		PA	     PAR      DAR      FPS           Scan     Note

    NTSC DV (4:3)		720x480	     10:11     4:3     30000:1001    i
    NTSC DV (16:9)		720x480	     40:33    16:9     30000:1001    i

    PAL DV (4:3)		720x576	     59:54     4:3     25:1	     i
    PAL DV (16:9)		720x576	    118:81    16:9     25:1	     i

    HDV 1080i60		1440x1080     4:3     16:9     30000:1001    i
    HDV 1080p30		1440x1080     4:3     16:9     30000:1001    p        Optional mode
    HDV 720p60		1280x720      1:1     16:9     60000:1001    p
    HDV 720p30		1280x720      1:1     16:9     30000:1001    p

    HDV 1080i50		1440x1080     4:3     16:9     25:1	     i
    HDV 1080p25		1440x1080     4:3     16:9     25:1	     p        Optional mode
    HDV 720p50		1280x720      1:1     16:9     50:1	     p
    HDV 720p25		1280x720      1:1     16:9     25:1	     p

    HDV 1080p24		1440x1080     4:3     16:9     24000:1001    p        Optional mode
    HDV 720p24		1280x720      1:1     16:9     24000:1001    p

    DVCPRO HD 1080i60	1280x1080     3:2     16:9     30000:1001    i
    DVCPRO HD 1080p30	1280x1080     3:2     16:9     30000:1001    p
    DVCPRO HD 720p60	960x720	      4:3     16:9     60000:1001    p
    DVCPRO HD 720p30	960x720	      4:3     16:9     30000:1001    p

    DVCPRO HD 1080i50	1440x1080     4:3     16:9     25:1          i
    DVCPRO HD 1080p25	1440x1080     4:3     16:9     25:1          p
    DVCPRO HD 720p50	960x720	      4:3     16:9     50:1	     p
    DVCPRO HD 720p25	960x720	      4:3     16:9     25:1	     p

    DVCPRO HD 1080pA24	1280x1080     3:2     16:9     24000:1001    p
    DVCPRO HD 720p24	960x720	      4:3     16:9     24000:1001    p

HDV
- <http://en.wikipedia.org/wiki/HDV>
- <http://documentation.apple.com/en/finalcutpro/professionalformatsandworkflows/chapter_2_section_1.html>

DVCPRO HD 
- <http://documentation.apple.com/en/finalcutpro/professionalformatsandworkflows/chapter_3_section_1.html>


Bibliography
------------

- <http://lurkertech.com/lg/video-systems/>
- <http://lurkertech.com/lg/pixelaspect/>
- <http://www.mir.com/DMG/aspect.html>
- <http://www.adamwilt.com/DV-tech.html>
- ITU-R Rec. BT.601
- SMPTE RP 187-1995
