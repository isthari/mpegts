#include "isthari.h"

//AAC audio
#define TYPE_AUDIO 0x0f
//H.264 video
#define TYPE_VIDEO 0x1b

//Audio PID
#define AUDIO_PID 257
//Video PID
#define VIDEO_PID 256
//PMT PID
#define PMT_PID 100

void muxOutput(SimpleBuffer &rTsOutBuffer, tsOutCallback callback){
    std::cout << "frame " << rTsOutBuffer.size() << std::endl;
    callback(rTsOutBuffer.data(),rTsOutBuffer.size());
}

MpegTsMuxer* createMuxer(tsOutCallback callback) {
    std::map<uint8_t, int> streamPidMap;
    streamPidMap[TYPE_AUDIO] = AUDIO_PID;
    streamPidMap[TYPE_VIDEO] = VIDEO_PID;

    std::cout << "createMuxer" << std::endl;
    MpegTsMuxer* muxer = new MpegTsMuxer(streamPidMap, PMT_PID, VIDEO_PID, MpegTsMuxer::MuxType::segmentType);    

    muxer->tsOutCallback = std::bind(&muxOutput, std::placeholders::_1, callback);
    //muxer->tsOutCallback = std::bind(callback, std::placeholders::_1);
    return muxer;
}

void addVideoFrame(MpegTsMuxer* pointer, const uint8_t* bytes, int size, uint64_t timestamp) {
    std::cout << "addVideoFrame" << std::endl;

    EsFrame esFrame;
    esFrame.mData = std::make_shared<SimpleBuffer>();
    //Append your ES-Data
    esFrame.mData->append(bytes, size);
    esFrame.mPts = timestamp;
    esFrame.mDts = timestamp;
    esFrame.mPcr = timestamp;
    esFrame.mStreamType = TYPE_VIDEO;
    esFrame.mStreamId = 224;
    esFrame.mPid = VIDEO_PID;
    esFrame.mExpectedPesPacketLength = 0;
    esFrame.mCompleted = true;

    //Multiplex your data
    pointer->encode(esFrame);

}