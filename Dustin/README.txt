Intelligent Camera System Project
=================================

1) Project Description
======================
    This project seeks to provide an intelligent solution to the issue of 
streaming video over an uncertain channel. Current solutions such as Netflix and
Amazon video answer this question by employing Adaptive Bitrate streaming, where
the end user's received video bitrate will vary depending on channel conditions.
While functional, this implementation fails to allow the user to select their 
own options for video scaling such as temporal or spatial scalability. Our 
system allows the user to select different scalability options to provide the 
user felxibility in various contexts. Future work will create a profile for the 
user in order to determine the user's priority for scaling options and decide
for the user when channel conditions change.

2) Project Hierarchy
====================
    The project is broken down into two applications: a video client (under the 
directory 'IntelligentCameraSystem') which is a desktop application written in 
C++ using the Qt application framework, and a streaming server (under the 
directory 'IntelligentCameraServer') which is an Android application designed to
be run on a DragonBoard APQ8060A. 

3) Environment Setup
====================
    It is recommended to be running Ubuntu linux version 10.04, 11.10, or 12.04 
for developing on this project. For the video client, it is best to use Ubuntu
12.04 due to dependency on versions of LibVLC  > 2.0.8.

    a) Video Client
    ---------------
    
    Developers should download the latest version of the Qt framework (version 
5.1.1 at the writing of this document) and can use Qt Creator as their IDE. To 
load the project, open Qt Creator, select 'File > Open File or Project...' and 
navigate to the directory IntelligentCameraSystem and select 
IntelligentCameraSystem.pro. You will most likely be prompted to choose a 
directory to build the executable. Choose any directory you wish.

    Developers should set the following environment variable so that necessary
files can be found:
 
INTELLIGENT_CAMERA_SYSTEM_ROOT_DIR=path-to-git-clone/Dustin/IntelligentCameraSystem

The build is dependent on the following libraries:
    LibVLC
    Dlib
    ffmpeg
LibVLC and Dlib (for linux) are packaged in the git repository under the 
directory 'IntelligentCameraSystem/libs/linux/'. For best results, the developer
may want to acquire LibVLC and ffmpeg for themself. On Ubuntu, to get LibVLC,
execute the following command:

    sudo apt-get install vlc libvlc-dev
    
Enter the directory /usr/lib/vlc/plugins/gui and remove the file libqt4_plugin.so.
To get ffmpeg, follow this link: http://www.ffmpeg.org/download.html. 

NOTE: ffmpeg is not necessary for the current version of the application. The 
classes using ffmpeg libraries have been excluded from the project, and all code
that would use these classes has been removed from the build via a macro. The 
reason why ffmpeg used to be a dependency is that we were investigating using
it as a media player instead of VLC but ultimately went with VLC. However, the 
code has been left in the application and the files defining the classes are 
left in their same directories in case we wish to test with ffmpeg again:
	UI/ffmpegvideowidget.c
	UI/ffmpegvideowidget.h
	Video/ffmpegwrapper.h
	Video/ffmpegwrapper.c
	Video/demuxer.h
	Video/demuxer.c 
To reinstate ffmpeg, first add these files back to the project. Next, go to the 
file UI/videowindow.h and change the value of the macro PLAY_WITH_VLC to 0. 
Finally, in the file UI/videowindow.ui, change the central widget from type 
VLCVideoWidget to FFMPEGVideoWidget. You can do this by opening videowindow.ui
in Qt Creator and editing it using the design pane.


The video client has not been recently tested on Windows, so instructions for
setting up the environment on windows will come at a later time.

    b) Camera Server
    
