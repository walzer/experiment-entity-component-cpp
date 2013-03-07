OpenGL_ES2_Samples
==================
Envirenment
-----------
Windows platform, installed tools below:
* jdk
* android sdk api level 17
    * If want use AVD to debug application, there is a way to accelerate AVD running, install necessary packages, "Extras/Inter x86 Emuator Accelerator" and "Android-XX/Intel x86 Atom System Image", and config APP\_ABI := x86 in Application.mk. 
* ndk r8d
    * Ensure ndk's path configure is correct in Eclipse->windows->preferences->Android->NDK.
* eclipse with ADT and CDT installed

Build
-----
In Eclipse:
1. Click **File->Project...->Android/Create project from existing source**
2. In the **Location** field, click **Browse...** and select the sample directory which want to build.
3. Right click project in packages explorer, then click **Android Tools->Add Native Support...**
4. Input "Sample" as library name, click Finish button.
5. Build and run project as you wish.
