//
// Created by 王强 on 2019/3/14.
//

#include "CallJava.h"

CallJava::CallJava(JavaVM *javaVM, JNIEnv *env, jobject *obj) {
    this->javaVM = javaVM;
    this->jniEnv = env;
    this->jobj = *obj;
    this->jobj = env->NewGlobalRef(jobj);

    jclass  clazz = jniEnv->GetObjectClass(jobj);
    if(!clazz) {
        if(LOG_ERROR) {
            LOGE("获取JavaClass失败");
        }
        return;
    }

    jmid_prepared = env->GetMethodID(clazz,"onCallPrepared", "()V");
    jmid_load = env->GetMethodID(clazz,"onCallLoad", "(Z)V");
    jmid_timeinfo = env->GetMethodID(clazz,"onCallTimeInfo", "(II)V");
    jmid_error=env->GetMethodID(clazz,"onCallError","(ILjava/lang/String;)V");
    jmid_complete=env->GetMethodID(clazz,"onCallComplete","()V");
    jmid_renderyuv=env->GetMethodID(clazz,"onCallRenderYUV","(II[B[B[B)V");
}

CallJava::~CallJava() {

}

void CallJava::onCallPrepared(int type) {
    if(type == MAIN_THREAD) {
        jniEnv->CallVoidMethod(jobj,jmid_prepared);
    }
    else if(type == CHILD_THREAD) {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0)!=JNI_OK) {
            if(LOG_ERROR) {
                LOGE("获取子线程失败");
            }
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_prepared);
        javaVM->DetachCurrentThread();
    }
}

void CallJava::onCallLoad(int type, bool load) {
    if(type == MAIN_THREAD) {
        jniEnv->CallVoidMethod(jobj, jmid_load, load);
    }
    else if(type == CHILD_THREAD){
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
            if(LOG_ERROR) {
                LOGE("call onCallLoad worng");
            }
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_load, load);
        javaVM->DetachCurrentThread();
    }
}

void CallJava::onCallTimeInfo(int type, int curr, int total) {
    if(type == MAIN_THREAD) {
        jniEnv->CallVoidMethod(jobj, jmid_timeinfo, curr, total);
    }
    else if(type == CHILD_THREAD) {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
            if(LOG_ERROR) {
                LOGE("call onCallTimeInfo worng");
            }
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_timeinfo, curr, total);
        javaVM->DetachCurrentThread();
    }
}

void CallJava::onCallError(int type, int code, char *msg) {
    if(type == MAIN_THREAD) {
        jstring jmsg = jniEnv->NewStringUTF(msg);
        jniEnv->CallVoidMethod(jobj, jmid_error, code, jmsg);
        jniEnv->DeleteGlobalRef(jmsg);
    }
    else if(type == CHILD_THREAD) {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
            if(LOG_ERROR) {
                LOGE("call onCallError worng");
            }
            return;
        }
        jstring jmsg = jniEnv->NewStringUTF(msg);
        jniEnv->CallVoidMethod(jobj, jmid_error, code, jmsg);
        jniEnv->DeleteGlobalRef(jmsg);
        javaVM->DetachCurrentThread();
    }
}

void CallJava::onCallComplete(int type) {
    if(type == MAIN_THREAD) {
        jniEnv->CallVoidMethod(jobj, jmid_complete);
    }
    else if(type == CHILD_THREAD) {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
            if(LOG_ERROR) {
                LOGE("call onCallComplete worng");
            }
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_timeinfo);
        javaVM->DetachCurrentThread();
    }
}

void CallJava::onCallRenderYUV(int width, int height, uint8_t *fy, uint8_t *fu, uint8_t *fv) {
    JNIEnv *jniEnv;
    if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
        if(LOG_ERROR) {
            LOGE("call onCallComplete worng");
        }
        return;
    }
    jbyteArray y=jniEnv->NewByteArray(width*height);
    jniEnv->SetByteArrayRegion(y, 0,width*height, reinterpret_cast<const jbyte *>(fy));
    jbyteArray u=jniEnv->NewByteArray(width*height/4);
    jniEnv->SetByteArrayRegion(u, 0,width*height/4, reinterpret_cast<const jbyte *>(fu));
    jbyteArray v=jniEnv->NewByteArray(width*height/4);
    jniEnv->SetByteArrayRegion(v, 0,width*height/4, reinterpret_cast<const jbyte *>(fv));
    jniEnv->CallVoidMethod(jobj,jmid_renderyuv,width,height,y,u,v);
    jniEnv->DeleteLocalRef(y);
    jniEnv->DeleteLocalRef(u);
    jniEnv->DeleteLocalRef(v);
    javaVM->DetachCurrentThread();
}

