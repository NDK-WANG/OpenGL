//
// Created by 王强 on 2019/3/14.
//

#ifndef OPENCAR_SAFEQUEUE_H
#define OPENCAR_SAFEQUEUE_H
#include <queue>
#include "AndroidLog.h"
#include "PlayStatus.h"
#include <pthread.h>
extern "C"{
#include <libavcodec/avcodec.h>
}

class SafeQueue {
public:
    std::queue<AVPacket *> queuePacket;
    std::queue<AVFrame *> queueFrame;
    pthread_mutex_t mutexPacket;
    pthread_cond_t condPacket;
    PlayStatus *playstatus = NULL;
    pthread_mutex_t mutexFrame;
    pthread_cond_t condFrame;
public:

    SafeQueue(PlayStatus *playstatus);
    ~SafeQueue();
    int putAvpacket(AVPacket *packet);
    int getAvpacket(AVPacket *packet);
    int putAvframe(AVFrame *frame);
    int getAvframe(AVFrame *frame);
    int getQueueSize();
    int getQueueframeSize();
    void clearAvpacket();
    void clearAvframe();
};


#endif //OPENCAR_SAFEQUEUE_H
