//
// Created by 王强 on 2019/3/14.
//

#ifndef OPENCAR_CALLJAVA_H
#define OPENCAR_CALLJAVA_H
#include "jni.h"
#include <linux/stddef.h>
#include "AndroidLog.h"
#include<string>
#define MAIN_THREAD 0
#define CHILD_THREAD 1

class CallJava {
public:
    JavaVM *javaVM = NULL;
    JNIEnv *jniEnv = NULL;
    jobject jobj;

    jmethodID jmid_prepared;
    jmethodID jmid_load;
    jmethodID jmid_timeinfo;
    jmethodID jmid_error;
    jmethodID jmid_complete;
    jmethodID jmid_renderyuv;
public:
    CallJava(JavaVM *javaVM,JNIEnv *env,jobject *obj);
    ~CallJava();

    void onCallPrepared(int type);

    void onCallLoad(int type, bool load);

    void onCallTimeInfo(int type, int curr, int total);

    void onCallError(int type,int code,char *msg);

    void onCallComplete(int type);

    void onCallRenderYUV(int width,int height,uint8_t *fy,uint8_t *fu,uint8_t *fv);
};


#endif //OPENCAR_CALLJAVA_H
