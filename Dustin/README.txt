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
felxibility in various contexts. Future work will create a profile for the 
user in order to determine the priority for scaling options and make intelligent
decisions when channel conditions change.

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
LibVLC and Dlib (for linux) are packaged in this git repository under the 
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
    ----------------
    Developers should have installed both the Android SDK and Android NDK. You 
can either download the SDK tools and integrate them with an existing version of
Eclipse or download the ADT bundle provided by Google. The SDK version that this
application uses is version 4.0.3 (API level 16). The NDK is necessary for 
compiling the JNI code used in the application. JNI is necessary in order to 
interface with the IOMX software stack that allows us to use hardware codecs.

    One must also have compatible hardware in order to run the application 
successfully. In our lab we are using a DragonBoard APQ8060A manufactured by 
Intrinsyc. Theoretically any device using a Qualcomm APQ8060A processor (msm8960)
should be compatible, but we have not tested on other devices yet so cannot say
definitively.

4) Project Structure
====================

    a) Video Client
    ---------------
    The video client contains deprecated code that was used in the original 
implementation of the user interface but is no longer relevant to know about in
the current version; for this reason we will only discuss relevant modules and 
files. All file paths given will be relative to the video client root directory
('IntelligentCameraSystem').

        i) User Interface
        -----------------
    There are three main windows currently used in the application; a login window,
a control center window, and an unspecified amount of video windows. The code for
the login and control center windows can be found in the root directory of the 
video client and the code for the video window can be found under the directory UI.
The login window simply contains two line edits (for username and password) and
two buttons (for login and quit). For now, users can log in by providing the following
credentials:
    username: test
    password: password
The control center window contains two list boxes (for available and open cameras)
and a button for opening a camera. A note on how to populate the available cameras
list box: there is a simple xml parser utility class (defined under the directory
Util) which retrieves the camera server information from an xml file. To add/remove
cameras, one can edit the file Config/camera_list_example.xml, following the 
structure that is already in place. This file path is hardcoded in the application
in the file Global/Constants.h. 

    A video window contains two custom widgets; VLCVideoWidget and VideoControls.
The VideoControls widget contains a combo box for video resolution, two line edits
for frame rate and bitrate, and a send button which will inform the video window to
send a request to the server to scale the video. The VLCVideoWidget contains the
actual media player and provides a wrapper around VLC. The actual implementation 
of our VLC wrapper can be found under the directory Video.
    
