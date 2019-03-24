package com.example.opengl;

import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.SeekBar;
import android.widget.TextView;

import AVOpenGL.GLSurface;
import AVPlayer.Player;
import Bean.TimeInfoBean;
import Listener.OnCompleteListener;
import Listener.OnErrorListener;
import Listener.OnLoadListener;
import Listener.OnPreparedListener;
import Listener.OnTimeInfoListener;
import Log.myLog;
import Util.TimeUtil;


public class MainActivity extends AppCompatActivity{
    private Player player;
    private TextView tvTime;
    private GLSurface glSurface;
    private SeekBar seekBar;
    private  int position;
    private boolean seek=false;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tvTime = findViewById(R.id.tv_time);

        player = new Player();

        glSurface=findViewById(R.id.glSurfaceView);
        seekBar=findViewById(R.id.seekBar);
        player.setGlSurface(glSurface);

        player.setOnPreparedListener(new OnPreparedListener() {
            @Override
            public void onPrepared() {
                myLog.d("准备好了");
                player.Start();
            }
        });

        player.setOnLoadListener(new OnLoadListener() {
            @Override
            public void onLoad(boolean load) {
                if (load) {
                    myLog.d("正在努力加载");
                } else {
                    myLog.d("播放中");
                }
            }
        });

        player.setTimeInfoListener(new OnTimeInfoListener() {

            @Override
            public void onTimeInfo(TimeInfoBean timeInfoBean) {
                Message message = Message.obtain();
                message.what = 1;
                message.obj = timeInfoBean;
                handler.sendMessage(message);
            }
        });

        player.setOnErrorListener(new OnErrorListener() {
            @Override
            public void onError(int code, String msg) {
                myLog.d("code:" + code + " msg:" + msg);
            }
        });

        player.setOnCompleteListener(new OnCompleteListener() {
            @Override
            public void onComplete() {
                myLog.d("播放完成了");
            }
        });

        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                position=progress*player.getDuration()/100;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                seek=true;
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                player.seek(position);
                seek=false;
            }
        });

    }

    public void begin(View view) {
        player.setSource("/mnt/shared/Other/Love.mp4");
        player.Prepared();
    }

    public void stop(View view) {
        player.Stop();
    }

    public void pause(View view) {
        player.Pause();
    }

    public void resume(View view) {
        player.Resume();
    }

    public void next(View view) {
    }

    Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            if (msg.what == 1) {
                TimeInfoBean timeInfoBean = (TimeInfoBean) msg.obj;
                tvTime.setText(TimeUtil.secdsToDateFormat(timeInfoBean.getCurrentTime(), timeInfoBean.getTotalTime())
                        + "/" + TimeUtil.secdsToDateFormat(timeInfoBean.getTotalTime(), timeInfoBean.getTotalTime()));
                if(!seek&&timeInfoBean.getTotalTime()>0){
                    seekBar.setProgress(timeInfoBean.getCurrentTime()*100/timeInfoBean.getTotalTime());
                }
            }
        }
    };
}

