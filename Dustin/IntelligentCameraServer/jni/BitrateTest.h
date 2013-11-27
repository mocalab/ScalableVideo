/*==============================================================================
            Copyright (c) 2011 QUALCOMM Incorporated.
            All Rights Reserved.
            Qualcomm Proprietary

	File: BitrateTest.h

	This class is used to illustrate how to further take advantage of the
	provided sample code to adapt different tests to it.

==============================================================================*/
#pragma once

#ifndef _BITRATE_TEST_H
#define _BITRATE_TEST_H

#include "QcomOmxPublicInterface.h"

// This function outputs encoded data to a file, but also keeps track of the number of frames
// written.  After a certain number of frames is written, it sends a command to change the bitrate
// on the fly to illustrate how to programmatically update the bitrate in code.
//
// This function can be used as an output callback function regardless of how the input is provided.
// 
int handleOutputEncodedToFile(void *obj, void *buffer, size_t bufferSize, void *iomxBuffer, void *userData);

// By encoding from one file to another, analysis tools can show the bitrate change on a single
// consistent file.  Modification of the various bitrates and frame setpoints can be made in order
// to experiment with what kind of results will be achieved at various resolutions.
//
// To see the different types of results, this test can be integrated into a shell script
// or coded into an APK by way of the NDK.
//
int bitrate_test(const char *fileFrom, const char *fileTo, int width, int height);

#endif
