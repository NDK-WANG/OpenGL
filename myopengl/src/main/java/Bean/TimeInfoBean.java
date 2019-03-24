package Bean;
/**
 * 时间的Bean类
 */
public class TimeInfoBean {
    private int currentTime;
    private int totalTime;

    public int getCurrentTime() {
        return currentTime;
    }

    public void setCurrentTime(int currentTime) {
        this.currentTime = currentTime;
    }

    public int getTotalTime() {
        return totalTime;
    }

    public void setTotalTime(int totalTime) {
        this.totalTime = totalTime;
    }

    @Override
    public String toString() {
        return "WlTimeInfoBean{" +
                "currentTime=" + currentTime +
                ", totalTime=" + totalTime +
                '}';
    }
}
