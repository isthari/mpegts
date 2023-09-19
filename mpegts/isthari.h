#pragma once

#include "mpegts_muxer.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*tsOutCallback) (uint8_t* data, int size);

MpegTsMuxer * createMuxer(tsOutCallback callback);
void addVideoFrame(MpegTsMuxer* pointer, const uint8_t* bytes, int size, uint64_t timestamp);

#ifdef __cplusplus
}
#endif