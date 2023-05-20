#ifndef SRMBUFFER_H
#define SRMBUFFER_H

#include "SRMTypes.h"
#include <GLES2/gl2.h>

#ifdef __cplusplus
extern "C" {
#endif

enum SRM_BUFFER_CAP
{
    SRM_BUFFER_CAP_READ = 1,
    SRM_BUFFER_CAP_WRITE = 2,
    SRM_BUFFER_CAP_MAP = 4
};

enum SRM_BUFFER_SRC
{
    SRM_BUFFER_SRC_CPU,
    SRM_BUFFER_SRC_WL_DRM
};

SRMBuffer *srmBufferCreateFromCPU(SRMCore *core,
                                  UInt32 width,
                                  UInt32 height,
                                  UInt32 stride,
                                  const void *pixels,
                                  SRM_BUFFER_FORMAT format);

SRMBuffer *srmBufferCreateFromWaylandDRM(SRMCore *core, void *wlBuffer);

UInt8 srmBufferWrite(SRMBuffer *buffer,
                      UInt32 stride,
                      UInt32 dstX,
                      UInt32 dstY,
                      UInt32 dstWidth,
                      UInt32 dstHeight,
                      const void *pixels);

SRM_BUFFER_FORMAT srmBufferGetFormat(SRMBuffer *buffer);
UInt32 srmBufferGetWidth(SRMBuffer *buffer);
UInt32 srmBufferGetHeight(SRMBuffer *buffer);

GLuint srmBufferGetTextureID(SRMDevice *device, SRMBuffer *buffer);
void srmBufferDestroy(SRMBuffer *buffer);


#ifdef __cplusplus
}
#endif

#endif // SRMBUFFER_H
