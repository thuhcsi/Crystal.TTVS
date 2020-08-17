
Modified on 20081103

The bmp/tga files have all been resized to be "MIPMAP" so that the width/height would be 2^n (where n is some positive integer).

The new width/height could be 1024, 512, 256, 128, 64, etc.

The "MIPMAP" is required by texture mapping (glTexImage2D for Embedded version only).


The last version of un-resized bmp/tga files could be found on the SVN Revision 162.

