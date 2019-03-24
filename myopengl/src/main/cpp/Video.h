



#include "SafeQueue.h"
#include "CallJava.h"
#include "Audio.h"

extern "C"
{
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/time.h>
#include <libavcodec/avcodec.h>
};

class Video {

public:
    int streamIndex = -1;
    AVCodecContext *avCodecContext = NULL;
    AVCodecParameters *codecpar = NULL;
    SafeQueue *queue = NULL;
    PlayStatus *playstatus = NULL;
    CallJava *callJava = NULL;
    AVRational time_base;
    pthread_t thread_play;
    Audio *audio = NULL;
    double clock = 0;
    double delayTime = 0;
    double defaultDelayTime = 0.04;
    pthread_mutex_t codecMutex;

public:
    Video(PlayStatus *playstatus, CallJava *callJava);
    ~Video();

    void play();

    void release();

    double getFrameDiffTime(AVFrame *avFrame);

    double getDelayTime(double diff);




};

