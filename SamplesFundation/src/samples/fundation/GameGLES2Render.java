package samples.fundation;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView.Renderer;

public class GameGLES2Render implements Renderer {

    public GameGLES2Render(Context context) {
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        NativeWrapper.GameGLES2Render.onDrawFrame();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        NativeWrapper.GameGLES2Render.onSurfaceChanged(width, height);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        NativeWrapper.GameGLES2Render.onSurfaceCreated();
    }

}
