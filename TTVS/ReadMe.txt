
CSTHead.vcproj: For Win32 DLL

CSTHeadCE.vcproj: For WinCE (Pocket PC 2003) DLL


(1) Deployment

(1.1) Remember to add the additional dll files in the deplyment setting.
Necessary files include:
libGLES.dll for OpenGL|ES (OpenGL on Embedded Systems)
msvcr80.dll (msvcr80d.dll) for msvc dll supporting.

(1.2) The recommended syntax for deploying necessary files:
msvcr80.dll|$(BINDIR)\$(INSTRUCTIONSET)\|%CSIDL_PROGRAM_FILES%\TTVS|0
msvcr80d.dll|$(BINDIR)\$(INSTRUCTIONSET)\|%CSIDL_PROGRAM_FILES%\TTVS|0
libGLES_CM.dll|$(SolutionDir)\..\ogles\bin\|%CSIDL_PROGRAM_FILES%\TTVS|0


(9) TODO and Done

(9.0) BUG to fix for Embedded System version
目前嵌入式版本，在渲染时会出现“花屏”的现象。
(a) 对于文件song_tongue.fdp，可以通过更改OpenGL的初始化设置（在OpenGL.cpp的setupDefaultEnvironment函数中加上glEnable(GL_CULL_FACE)）解决该问题；
(b) 对于song_teeth.fdp，问题仍然存在。问题主要表现在两个不同的Object在有重叠时，本应被遮蔽的Object在渲染显示时有些部分会时不时露出来，导致“花屏”。初步怀疑是深度测试Depth Test的问题，但是在setupDefaultEnvironment函数中已经通过glEnable(GL_DEPTH_TEST)打开了深度测试的功能，结果还是不行。问题未能解决。不知是否和OpenGL|ES的初始化有关？比如可以通过eglChooseConfig来选择特定的config信息？
(c) 透明显示似乎也存在一定的问题。比如对于song_teeth.fdp，在牙齿边缘处应是透明的、并可显示被遮蔽的下牙（可在桌面版本的测试程序中打开观察），但是在嵌入式版本的程序中透明部分虽显示背景色、但下牙并未显示出来。可能是Alpha Test的问题。

(9.1) TODO

(a) Determine whether OpenGLBmp can be removed, and use OpenGLW directly.
When the input parameter (HWND) is NULL for OpenGLW initialization, Offscreen rendering will be performed

(b) TalkingHeadGL, TalkingHeadConcatenate?

(9.2) 20080901 Done
Add the function to unload model (close the opened model).
So that the user could load a new model after unloading (closing) current model.

(9.3) 20081028 Done
Make the OpenGL, CSTTalkingHead, and AnimationPlayer independent.
(a) OpenGL
(b) CSTTalkingHead -> render(), rewind(), next(), generateAnimation(), getFrameData(), getFrameSize()
(c) AnimationPlayer -> initialize(CSTTalkingHead, HWND), play(Start new thread), pause, resume, stop
(d) VideoGenerator -> generateVideo(waveFile, ctxtFile...)
