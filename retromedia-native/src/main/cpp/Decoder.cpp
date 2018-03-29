//
// Created by jpxiong on 2018/2/7.
//
#define LOG_TAG "Decoder.cpp"
#include "Decoder.h"

#include <chrono>
#include <ratio>
#include <list>
#include "common/log_defs.h"
#include "common/Buffer.h"
#include <unistd.h>

#define ALIGN_EDGE 16
#define ENABLE_FPS_ESTIMATE

#ifdef ENABLE_FPS_ESTIMATE
class EstimateModule {
private:
    std::list<int64_t> mList;
    int mIntervalWindowMs = 5000;
    int mIntervalMaxNum = 120 * 5;
public:
    EstimateModule() {
        mList.clear();
    }

    void checkAndErase(int64_t ms) {
        if (mList.size() > mIntervalMaxNum) {
            mList.pop_front();
        }

        int64_t endTime = ms;
        int64_t startTime = ms - mIntervalWindowMs;

        std::list<int64_t>::iterator item = mList.begin();
        while (item != mList.end()) {
            if (*item < startTime) {
                mList.erase(item);
                item = mList.begin();
            }else {
                break;
            }
        }
    }

    void push(int64_t ms) {
        checkAndErase(ms);
        mList.push_back(ms);
    }

    double rate(int64_t ms) {
        checkAndErase(ms);
        double size = mList.size();

        int64_t diffMs = ms - (*mList.begin());

        return diffMs > 0 ? size * 1000 / diffMs : 0;
    }
};
static EstimateModule sEstimateFps;

#endif //ENABLE_FPS_ESTIMATE

Decoder::Decoder() {
    mFormatCtx = NULL;
    mAudioCodec = NULL;
    mVideoCodec = NULL;
    mVideoCodecCtx = NULL;
    mAudioCodecCtx = NULL;
    mVideoStreamIdx = mAudioStreamIdx = -1;
    mVideoFrame = NULL;
    mBackBuffer = mFrontBuffer = NULL;
    mSeek = -1;
    mPause = true;
    mStart = false;
    mSpeed = 1.0f;
    mBufferPool = NULL;
    ALOGD(" Decoder Ctor %p", this);
}

void Decoder::initFFmpeg() {
    av_register_all();
    avcodec_register_all();
    avformat_network_init();
    avfilter_register_all();
}

bool Decoder::start(const char *file) {
    initFFmpeg();

    bool ret = false;
    bool hasAudio = false;
    bool hasVideo = false;

    if (file == NULL || access(file, R_OK) < 0) {
        return ret;
    }

    if (avformat_open_input(&mFormatCtx, file, NULL, NULL) < 0)
        return ret;

    if (avformat_find_stream_info(mFormatCtx, NULL) < 0)
        return ret;

    av_dump_format(mFormatCtx, -1, file, 0);

    int i = 0;
    for (i = 0; i < mFormatCtx->nb_streams; ++i) {
        if (mFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            mVideoStreamIdx = i;
            mVideoCodecCtx = mFormatCtx->streams[mVideoStreamIdx]->codec;
        }

        if (mFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            mAudioStreamIdx = i;
            mAudioCodecCtx = mFormatCtx->streams[mVideoStreamIdx]->codec;
        }
    }

    if (mAudioCodecCtx) {
        mAudioCodec = avcodec_find_decoder(mAudioCodecCtx->codec_id);
    }

    if (mVideoCodecCtx) {
        mVideoCodec = avcodec_find_decoder(mVideoCodecCtx->codec_id);
    }

    if (mAudioCodec) {
        if (avcodec_open2(mAudioCodecCtx, mAudioCodec, NULL) >= 0) {
            hasAudio = true;
        }
    }

    if (mVideoCodec) {
        if (avcodec_open2(mVideoCodecCtx, mVideoCodec, NULL) >= 0) {
            hasVideo = true;
        }
    }

    if (!hasAudio) {
        mAudioStreamIdx = -1;
    }

    if (!hasVideo) {
        mVideoStreamIdx = -1;
    }

    ALOGD("has video %d has audio %d", (int)hasVideo, (int)hasAudio);
    mStart = hasAudio | hasVideo;

    if (mStart) {
        initVideoBuffer();
        pause();
        mDecodeThread = new std::thread(&Decoder::decodeThread, this);
    }
    return mStart;
}

bool Decoder::stop() {
    mStart = false;

    {
        std::lock_guard<std::mutex> lock_guard(mRenderLock);
        mRenderCond.notify_all();
    }

    if (mDecodeThread != NULL) {
        mDecodeThread->join();
        delete mDecodeThread;
        mDecodeThread = NULL;
    }

    std::lock_guard<std::mutex> lock_guard(mMutex);

    if (mVideoCodecCtx) {
        avcodec_close(mVideoCodecCtx);
        av_free(mVideoCodecCtx);
        mVideoCodecCtx = NULL;
        mVideoCodec = NULL;
    }

    if (mAudioCodecCtx) {
        avcodec_close(mAudioCodecCtx);
        av_free(mAudioCodecCtx);
        mAudioCodecCtx = NULL;
        mAudioCodec = NULL;
    }

    if (mFormatCtx) {
        avio_close(mFormatCtx->pb);
        mAudioStreamIdx = -1;
        mVideoStreamIdx = -1;
        av_free(mFormatCtx);
        mFormatCtx = NULL;
    }

    if (mVideoFrame) {
        av_frame_free(&mVideoFrame);
        mVideoFrame = NULL;
    }

    deleteVideoBuffer(&mFrontBuffer);
    deleteVideoBuffer(&mBackBuffer);

    mFrontBuffer = NULL;
    mBackBuffer = NULL;

    ALOGD("Free DecodeLite done.");
    return true;
}

int Decoder::getAVPacket(AVPacket *packet, bool videoPacket) {
    int ret = 0;
    int packetType = videoPacket ? mVideoStreamIdx : mAudioStreamIdx ;
    av_init_packet(packet);

    do  {
        av_free_packet(packet);
        av_init_packet(packet);
        ret = av_read_frame(mFormatCtx, packet);
    }while ((mFormatCtx && packetType >= 0 && packet->stream_index != packetType && ret >= 0));

    if (ret < 0) {
        av_free_packet(packet);
    }
    return ret;
}

int Decoder::seek(long timeMs) {
    mSeek = timeMs;
    return 0;
}

int Decoder::width() {
    int w = 0;
    if (mVideoCodecCtx) {
        w = mVideoCodecCtx->width;
    }
    return w;
}

int Decoder::height() {
    int h = 0;
    if (mVideoCodecCtx) {
        h = mVideoCodecCtx->height;
    }
    return h;
}

int Decoder::stride() {
    return FFALIGN(width(), ALIGN_EDGE);
}

VideoBuffer *Decoder::genVideoBuffer(VideoBuffer **vframe) {
    VideoBuffer *frame = NULL;
    *vframe = mBufferPool->pollVideoBuffer();
    return *vframe;
}

int Decoder::getVideoBuffer(VideoBuffer * & frame) {
    std::lock_guard<std::mutex> lock_guard(mMutex);
    if (frame && mFrontBuffer && mFrontBuffer->size() > 0) {
        std::swap(frame, mFrontBuffer);
    }

    return frame != NULL && frame->size() > 0 ? 0 : -1;
}

AudioBuffer *Decoder::getAudioFrame() {
    return NULL;
}

Decoder::~Decoder() {
    stop();
    ALOGD("~Dtor Decoder this %p", this);
}

void Decoder::decodeThread() {
    VideoBuffer *frame = NULL;
    int64_t duration = 16000;
    int64_t startTimeFps = 0;
    mStartTime = 0;

    while (mStart) {
        if (mSeek >= 0) {
            seekInternal(mSeek);
            mSeek = -1;
            mStartTime = 0;
        }

        {
            std::unique_lock<std::mutex> lock(mRenderLock);
            mRenderCond.wait_for(lock, std::chrono::duration<int64_t, std::micro>(duration));
            if (!mStart) {
                break;
            }

            if (mPause) {
                continue;
            }
        }

        int64_t decodeTime = av_gettime();
        frame = decodeFrame(genVideoBuffer(&mBackBuffer));

        if (!frame) {
            break;
        }

        int64_t currTimeBase = frame->mTimestamp;
        swapBuffer();

        int64_t decodeTimeDiff = av_gettime() - decodeTime;

        {
            std::lock_guard<std::mutex> lock_guard(mMutex);
            if (mStartTime != 0) {
                duration = currTimeBase - mStartTime - decodeTimeDiff;
            }else {
                duration = 0;
            }

            duration = mSpeed > 0 ? mSpeed * duration : duration;
            mStartTime = currTimeBase;
        }

        int64_t currTimeMs = av_gettime() / 1000;

#ifdef ENABLE_FPS_ESTIMATE
        sEstimateFps.push(currTimeMs);
        if ((currTimeMs - startTimeFps) / 1000 > 1) {
            ALOGD("decode fps %0.2f fps", sEstimateFps.rate(currTimeMs));
            startTimeFps = currTimeMs;
        }
#endif
    }
}

VideoBuffer *Decoder::decodeFrame(VideoBuffer * frame) {
    int ret = -1;
    int isFinish = 0;

    if (!mStart) {
        return NULL;
    }

    if (!frame) {
        return NULL;
    }

    AVPacket packet;

    if (mVideoFrame == NULL) {
        mVideoFrame = avcodec_alloc_frame();
    }

    while (getAVPacket(&packet, true) >= 0) {
        ret = avcodec_decode_video2(mVideoCodecCtx, mVideoFrame, &isFinish, &packet);
        if (ret < 0 || !isFinish) {
            continue;
        }

        AVPicture *pic = (AVPicture *)mVideoFrame;
        ret = av_image_copy_to_buffer(frame->data(), frame->size(), (const uint8_t * const*)pic->data,
                                      pic->linesize, PIX_FMT_YUV420P, mVideoCodecCtx->width, mVideoCodecCtx->height,
                                      ALIGN_EDGE);

        //frame->size = mVideoCodecCtx->width * mVideoCodecCtx->height * 3 / 2;
        frame->mTimestamp = av_rescale_q(mVideoFrame->pkt_pts,
                                        mFormatCtx->streams[mVideoStreamIdx]->time_base,
                                        (AVRational) {1, AV_TIME_BASE});
        frame->mDuratioin = mVideoFrame->pkt_duration;
        break;
    }

    if (ret < 0) {
        frame = NULL;
        ALOGW("Invalid frame !");
    }

    return frame;
}

void Decoder::pause() {
    mPause = true;
}

void Decoder::resume() {
    mPause = false;
}

void Decoder::initVideoBuffer() {

    if (mBufferPool == nullptr) {
        mBufferPool = new VideoBufferPool(10, stride() * height() * 3 / 2, "VideoDecoderPool");
    }
    // Decode First frame
    decodeFrame(genVideoBuffer(&mBackBuffer));
    mStartTime = mBackBuffer->mTimestamp;
    swapBuffer();
}

void Decoder::swapBuffer() {
    std::lock_guard<std::mutex> lock_guard(mMutex);
    std::swap(mBackBuffer, mFrontBuffer);
}

void Decoder::deleteVideoBuffer(VideoBuffer **pFrame) {
    if (*pFrame) {
        (*pFrame)->decRef();
    }
}

void Decoder::seekInternal(long seek) {
    if (mFormatCtx != NULL && seek >=0 ) {
        av_seek_frame(mFormatCtx, -1, seek, 0);
    }
}

void Decoder::setSpeed(double d) {
    mSpeed = d;
}
