/*==============================================================================
            Copyright (c) 2011 QUALCOMM Incorporated.
            All Rights Reserved.
            Qualcomm Proprietary

	File: QcomOmxPublicInterface.h

==============================================================================*/
#pragma once

#ifndef _QCOM_OMX_RENDERER_H
#define _QCOM_OMX_RENDERER_H

#include "QcomOmxPublicInterface.h"

class QcomRenderer;
QcomRenderer *omx_interface_create_renderer(void *obj, JNIEnv *env, jobject javaSurface, 
									int fileWidth, int fileHeight, 
									int outputWidth, int outputHeight, 
									int hwVersion = 0);

int omx_interface_render(void *obj, QcomRenderer *renderer, void *iomxBuffer);

int delete_renderer(QcomRenderer *renderer);

// omx_surface_render
//
//     This follows the _QcomOmxOutputCallback declaration.  The buffer parameter is a pointer
//     to the bufferInfo while the iomxBuffer parameter corresponds to the iomxBuffer item inside the buffer.
//
//     The iomxBuffer needs to be sent to the render() call, but the buffer encapsulation needs to be held onto
//     in order to be able to update state when it's time to send it back to the component.
//
int   omx_surface_render(void *obj, void *buffer, size_t bufferSize, void *iomxBuffer, void *userData);
int	  omx_surface_render_immediate(void *obj, void *buffer, size_t bufferSize, void *iomxBuffer, void *userData);

#endif
