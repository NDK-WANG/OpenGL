package AVPlayer;

import android.text.TextUtils;

import AVOpenGL.GLSurface;
import Bean.TimeInfoBean;
import Listener.OnCompleteListener;
import Listener.OnErrorListener;
import Listener.OnLoadListener;
import Listener.OnPauseResumeListener;
import Listener.OnPreparedListener;
import Listener.OnTimeInfoListener;
import Log.myLog;

public class Player {
    //加载动态库
    static{
        System.loadLibrary("avcodec-57");
        System.loadLibrary("avdevice-57");
        System.loadLibrary("avfilter-6");
        System.loadLibrary("avformat-57");
        System.loadLibrary("avutil-55");
        System.loadLibrary("postproc-54");
        System.loadLibrary("swresample-2");
        System.loadLibrary("swscale-4");
        System.loadLibrary("native-lib");
    }
    private String source;//数据源
    private static TimeInfoBean timeInfoBean;
    private OnPreparedListener onPreparedListener;
    private OnLoadListener onLoadListener;
    private OnPauseResumeListener onPauseResumeListener;
    private OnTimeInfoListener onTimeInfoListener;
    private OnErrorListener onErrorListener;
    private OnCompleteListener onCompleteListener;
    private GLSurface glSurface;
    private int duration=0;

    public Player(){

    }

    //设置数据源
    public void setSource(String source) {
        this.source = source;
    }

    //回调接口
    public void setOnPreparedListener(OnPreparedListener onPreparedListener){
        this.onPreparedListener=onPreparedListener;
    }//准备回调

    public void setOnLoadListener(OnLoadListener onLoadListener){
        this.onLoadListener=onLoadListener;
    }//加载回调

    public void setOnPauseResumeListener(OnPauseResumeListener onPauseResumeListener){
        this.onPauseResumeListener=onPauseResumeListener;
    }//播放与暂停回调

    public void setTimeInfoListener(OnTimeInfoListener onTimeInfoListener) {
        this.onTimeInfoListener=onTimeInfoListener;
    }//播放时间回调

    public void setOnErrorListener(OnErrorListener onErrorListener) {
        this.onErrorListener = onErrorListener;
    }//出错的回调

    public void setOnCompleteListener(OnCompleteListener onCompleteListener) {
        this.onCompleteListener = onCompleteListener;
    }//完成播放的回调

    public void setGlSurface(GLSurface glSurface) {
        this.glSurface = glSurface;
    }//设置glsurface

    public int getDuration() {
        return duration;
    }//获得时间

    //播放的方法
    public void Prepared(){
        if(TextUtils.isEmpty(source)){
            myLog.d("资源为空");
            return;
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                n_prepared(source);
            }
        }).start();
    }//开始准备

    public void Start(){

        if(TextUtils.isEmpty(source)){
            myLog.d("资源为空");
            return;
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                //开始调用native方法
                n_start();
            }
        }).start();
    }//开始播放

    public void Pause(){
        //调用native层方法
        n_pause();
        if(onPauseResumeListener!=null){
            onPauseResumeListener.onPause(true);
        }
    }//暂停

    public void Resume(){
        //调用native层方法
        n_resume();
        if(onPauseResumeListener!=null){
            onPauseResumeListener.onPause(false);
        }
    }//继续播放

    public void Stop(){
        timeInfoBean=null;
        duration=0;
        new Thread(new Runnable() {
            @Override
            public void run() {
                //调用native层方法
                n_stop();
            }
        }).start();
    }//停止播放

    public void seek(int secds){
        n_seek(secds);
    }

    //c++调用Java方法
    public void onCallPrepared(){
        if(onPreparedListener!=null){
            onPreparedListener.onPrepared();
        }
    }

    public void onCallLoad(boolean load){
        if(onLoadListener!=null){
            onLoadListener.onLoad(load);
        }
    }

    public void onCallTimeInfo(int currentTime,int totalTime){
        if(onTimeInfoListener!=null){
            if(timeInfoBean==null){
                timeInfoBean=new TimeInfoBean();
            }
            duration=totalTime;
            timeInfoBean.setCurrentTime(currentTime);
            timeInfoBean.setTotalTime(totalTime);
            onTimeInfoListener.onTimeInfo(timeInfoBean);
        }
    }

    public void onCallError(int code,String msg){
        Stop();
        if(onErrorListener!=null){
            onErrorListener.onError(code,msg);
        }
    }

    public void onCallComplete(){
        Stop();
        if(onCompleteListener!=null){
            onCompleteListener.onComplete();
        }
    }

    public void onCallRenderYUV(int width,int height,byte[] y,byte[] u,byte[] v){
        myLog.d("获取到视频的YUV");
        if(glSurface!=null){
            glSurface.setYUVData(width,height,y,u,v);
        }
    }//呼叫java数据好了

    //native层方法
    private native void n_prepared(String source);
    private native void n_start();
    private native void n_pause();
    private native void n_resume();
    private native void n_stop();
    private native void n_seek(int secds);



}
