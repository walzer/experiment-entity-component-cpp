package samples.fundation;

import java.io.BufferedInputStream;
import java.io.IOException;

public class NativeWrapper {
    public static class GameGLES2Render {
        public static native void onDrawFrame();
        public static native void onSurfaceChanged(int width, int height);
        public static native void onSurfaceCreated();
    }
    
    public static class MotionEvent  {
        public static native boolean onPointerAction(
                int action, 
                int source,
                int count, 
                int[] idList,
                int[] toolTypeList, 
                float[] xList,
                float[] yList);
    }
    
    public static byte[] readDataFromAsset(String path) {
        byte[] data = null;
        BufferedInputStream bis;
        try {
            bis = new BufferedInputStream(
                    SampleApplication.getInstance().getAssets().open(path));
            data = new byte[bis.available()];
            bis.read(data);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return data;
    }
}
