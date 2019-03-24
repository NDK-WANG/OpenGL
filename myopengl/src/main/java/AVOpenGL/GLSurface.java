package AVOpenGL;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
public class GLSurface extends GLSurfaceView{
    private Render render;

    public GLSurface(Context context) {
        this(context, null);
    }

    public GLSurface(Context context, AttributeSet attrs) {
        super(context, attrs);
        setEGLContextClientVersion(2);
        render = new Render(context);
        setRenderer(render);
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }

    public void setYUVData(int width, int height, byte[] y, byte[] u, byte[] v)
    {
        if(render != null)
        {
            render.setYUVRenderData(width, height, y, u, v);
            requestRender();
        }
    }
}
