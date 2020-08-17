# Crystal Text-to-Visual-Speech (TTVS) Engine

### Compiling the Projects

1. Compile TinyXML
   - Open "TinyXML" solution in "TinyXML" directory
   - Set "tinyxmlSTL" project as "StartUp Project"
   - Set "Solution Platforms" to "Win32"
   - Compile "tinyxmlSTL" project in "Release" version with "Multi-threaded DLL (/MD)" option
   - Compile "tinyxmlSTL" project in "Debug" version (Optional) with "Multi-threaded debug DLL (/MDd)" option

2. Complie Xface
   - Open "XFace.sln" solution in "XFace" directory
   - Set "XFace" project as "StartUp Project"
   - Set "Solution Platforms" to "Win32"
   - Compile "Xface" project in "Release" version
   - Compile "Xface" project in "Debug" version

3. Compile TTVS
   - Open "TTVSTest.sln" solution in "TTVSTest" directory
   - Set "TTVSTest" project as "Startup Project"
   - Set "Solution Platforms" to "Win32"
   - Compile "TTVS" and "TTVSTest" project in "Release" version
   - Compile "TTVS" and "TTVSTest" project in "Debug" version

### Runing the demo

To run the demo
   - Go to "binary/win32" directory
   - Go to "release" or "debug" directory
   - Run TTVSTest.exe
   - Select menu "File/Open" to open the "fdp" file in "data/talkinghead/Song/song.fdp"
   - Select menu "TTVS/Mandarin", "TTVS/Cantonese", "TTVS/English" to switch between different langauges.
   - Select menu "TTVS/Screen Animation" for demo animation.