package samples.fundation;

public class NativeWrapper {
    public static class GameGLES2Render{
        public static native void onDrawFrame();
        public static native void onSurfaceChanged(int width, int height);
        public static native void onSurfaceCreated();
    }
}
