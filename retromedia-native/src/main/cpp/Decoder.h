//
// Created by jpxiong on 2018/2/7.
//

#ifndef SHORTVIDEOEDITOR_DUETMANAGER_H
#define SHORTVIDEOEDITOR_DUETMANAGER_H

#include <pthread.h>
#include <sys/time.h>
#include <deque>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include "VideoType.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include <libavutil/time.h>
#include <libswresample/swresample.h>

#include <libswscale/swscale.h>
#include <libswscale/swscale_internal.h>

#include <libavfilter/avfiltergraph.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>

#ifdef __cplusplus
}
#endif

#define AudioBuffer VideoBuffer

class Decoder {
private:
    AVFormatContext *mFormatCtx;
    int              mVideoStreamIdx;
    int              mAudioStreamIdx;
    AVCodecContext * mVideoCodecCtx;
    AVCodecContext * mAudioCodecCtx;
    AVCodec        * mVideoCodec;
    AVCodec        * mAudioCodec;
    AVFrame        * mVideoFrame;
    VideoBuffer     * mFrontBuffer;
    VideoBuffer     * mBackBuffer;
    std::mutex       mMutex;
    std::thread    * mDecodeThread;
    std::condition_variable mRenderCond;
    std::mutex       mRenderLock;
    bool             mPause;
    bool             mStart;
    int64_t          mStartTime;
    long             mSeek;
private:
    void initFFmpeg();

    void decodeThread();

    void swapBuffer();

    void initVideoBuffer();

    void seekInternal(long seek);

    VideoBuffer *decodeFrame(VideoBuffer *);

public:
    Decoder();

    bool start(const char *file);

    void pause();

    void resume();

    bool stop();

    int seek(long timeMs);

    int width();

    int height();

    int stride();

    int getVideoBuffer(VideoBuffer * & frame);

    void deleteVideoBuffer(VideoBuffer **pFrame);

    int getAVPacket(AVPacket *packet, bool videoPacket = true);

    VideoBuffer *genVideoBuffer(VideoBuffer **buffer);

    AudioBuffer *getAudioFrame();

    ~Decoder();

    void setSpeed(double d);

    double mSpeed;
};


#endif //SHORTVIDEOEDITOR_DUETMANAGER_H
