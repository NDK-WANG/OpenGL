//
// Created by 王强 on 2019/3/14.
//

#include "SafeQueue.h"
SafeQueue::SafeQueue(PlayStatus *playstatus) {
    this->playstatus = playstatus;
    pthread_mutex_init(&mutexPacket, NULL);
    pthread_cond_init(&condPacket, NULL);
    pthread_mutex_init(&mutexFrame,NULL);
    pthread_cond_init(&condFrame,NULL);
}

SafeQueue::~SafeQueue() {
    clearAvpacket();
    clearAvframe();
}

int SafeQueue::putAvpacket(AVPacket *packet) {

    pthread_mutex_lock(&mutexPacket);

    queuePacket.push(packet);
    pthread_cond_signal(&condPacket);
    pthread_mutex_unlock(&mutexPacket);

    return 0;
}

int SafeQueue::getAvpacket(AVPacket *packet) {

    pthread_mutex_lock(&mutexPacket);

    while(playstatus != NULL && !playstatus->exit)
    {
        if(queuePacket.size() > 0)
        {
            AVPacket *avPacket =  queuePacket.front();
            if(av_packet_ref(packet, avPacket) == 0)
            {
                queuePacket.pop();
            }
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            break;
        } else{
            pthread_cond_wait(&condPacket, &mutexPacket);
        }
    }
    pthread_mutex_unlock(&mutexPacket);
    return 0;
}

int SafeQueue::getQueueSize() {

    int size = 0;
    pthread_mutex_lock(&mutexPacket);
    size = queuePacket.size();
    pthread_mutex_unlock(&mutexPacket);

    return size;
}

void SafeQueue::clearAvpacket() {
    pthread_cond_signal(&condPacket);
    pthread_mutex_unlock(&mutexPacket);

    while (!queuePacket.empty())
    {
        AVPacket *packet = queuePacket.front();
        queuePacket.pop();
        av_packet_free(&packet);
        av_free(packet);
        packet = NULL;
    }
    pthread_mutex_unlock(&mutexPacket);

}

int SafeQueue::putAvframe(AVFrame *frame) {
    pthread_mutex_lock(&mutexPacket);

    queueFrame.push(frame);
    pthread_cond_signal(&condFrame);
    pthread_mutex_unlock(&mutexFrame);
    return 0;
}

int SafeQueue::getAvframe(AVFrame *frame) {
    pthread_mutex_lock(&mutexFrame);

    while(playstatus != NULL && !playstatus->exit)
    {
        if(queueFrame.size() > 0)
        {
            AVFrame *avFrame=queueFrame.front();
            if(av_frame_ref(frame,avFrame) == 0)
            {
                queueFrame.pop();
            }
            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = NULL;
            break;
        } else{
            pthread_cond_wait(&condFrame, &mutexFrame);
        }
    }
    pthread_mutex_unlock(&mutexFrame);
    return 0;
}

void SafeQueue::clearAvframe() {
    pthread_cond_signal(&condFrame);
    pthread_mutex_unlock(&mutexFrame);

    while (!queueFrame.empty())
    {
        AVFrame *frame=queueFrame.front();
        queueFrame.pop();
        av_frame_free(&frame);
        av_free(frame);
        frame = NULL;
    }
    pthread_mutex_unlock(&mutexFrame);
}

int SafeQueue::getQueueframeSize() {
    int size = 0;
    pthread_mutex_lock(&mutexFrame);
    size = queueFrame.size();
    pthread_mutex_unlock(&mutexFrame);

    return size;
}
