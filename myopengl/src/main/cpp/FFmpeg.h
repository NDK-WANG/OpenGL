//
// Created by yangw on 2018-2-28.
//

#ifndef MYMUSIC_WLFFMPEG_H
#define MYMUSIC_WLFFMPEG_H

#include "CallJava.h"
#include "pthread.h"
#include "Audio.h"
#include "PlayStatus.h"
#include "Video.h"

extern "C"
{
#include "libavformat/avformat.h"
#include <libavutil/time.h>
};


class FFmpeg {

public:
    CallJava *callJava = NULL;
    const char* url = NULL;
    pthread_t decodeThread;
    AVFormatContext *pFormatCtx = NULL;
    Audio *audio = NULL;
    Video *video = NULL;
    PlayStatus *playstatus = NULL;
    pthread_mutex_t init_mutex;
    bool exit = false;
    int duration = 0;
    pthread_mutex_t seek_mutex;
public:
    FFmpeg(PlayStatus *playstatus, CallJava *callJava, const char *url);
    ~FFmpeg();

    void prepared();
    void decodeFFmpegThread();
    void start();

    void pause();

    void resume();

    void release();

    void seek(int64_t secds);

    int getCodecContext(AVCodecParameters *codecpar, AVCodecContext **avCodecContext);

};


#endif //MYMUSIC_WLFFMPEG_H
