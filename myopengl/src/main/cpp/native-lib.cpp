#include <jni.h>
#include <string>
#include "CallJava.h"
#include "FFmpeg.h"
#include "PlayStatus.h"

extern "C"{
#include <libavformat/avformat.h>
}
JavaVM *javaVM=NULL;
CallJava *callJava = NULL;
FFmpeg *ffmpeg = NULL;
PlayStatus *playstatus = NULL;
bool next= true;
pthread_t pthread_start;
extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jint result = -1;
    javaVM = vm;
    JNIEnv *env;
    if(vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }
    return JNI_VERSION_1_4;

}


void *startCallBack(void *data){
    FFmpeg *ffmpeg= static_cast<FFmpeg *>(data);
    ffmpeg->start();
    pthread_exit(&pthread_start);
}
extern "C"
JNIEXPORT void JNICALL
Java_AVPlayer_Player_n_1start(JNIEnv *env, jobject instance) {
    // TODO
    if(ffmpeg != NULL) {
        pthread_create(&pthread_start,NULL,startCallBack,ffmpeg);
    }
}extern "C"
JNIEXPORT void JNICALL
Java_AVPlayer_Player_n_1pause(JNIEnv *env, jobject instance) {

    // TODO
    if(playstatus!=NULL){
        playstatus->pause=true;
    }
    if(ffmpeg != NULL) {
        ffmpeg->pause();
    }

}extern "C"
JNIEXPORT void JNICALL
Java_AVPlayer_Player_n_1stop(JNIEnv *env, jobject instance) {

    // TODO
    if(!next){
        return;
    }
    next= false;
    if(ffmpeg != NULL) {

        ffmpeg->release();
        delete(ffmpeg);
        ffmpeg = NULL;
        if(callJava != NULL) {
            delete(callJava);
            callJava = NULL;
        }
        if(playstatus != NULL) {
            delete(playstatus);
            playstatus = NULL;
        }
    }
    next= true;
}extern "C"
JNIEXPORT void JNICALL
Java_AVPlayer_Player_n_1resume(JNIEnv *env, jobject instance) {

    if(playstatus!=NULL){
        playstatus->pause= false;
    }
    // TODO
    if(ffmpeg != NULL) {
        ffmpeg->resume();
    }

}extern "C"
JNIEXPORT void JNICALL
Java_AVPlayer_Player_n_1seek(JNIEnv *env, jobject instance, jint secds) {

    // TODO
    if(ffmpeg!=NULL){
        ffmpeg->seek(secds);
    }

}extern "C"
JNIEXPORT void JNICALL
Java_AVPlayer_Player_n_1prepared(JNIEnv *env, jobject instance, jstring source_) {
    const char *source = env->GetStringUTFChars(source_, 0);
    if(ffmpeg == NULL) {
        if(callJava == NULL) {
            callJava = new CallJava(javaVM, env, &instance);
        }
        callJava->onCallLoad(MAIN_THREAD,true);
        playstatus = new PlayStatus();
        ffmpeg = new FFmpeg(playstatus, callJava, source);
        ffmpeg->prepared();
    }

}