package samples.fundation;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Build.VERSION;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;

public class GameView extends GLSurfaceView {

    public GameView(Context context) {
        super(context);

        this.initView(context);
    }

    public GameView(Context context, AttributeSet attrs) {
        super(context, attrs);

        this.initView(context);
    }

    private void initView(Context context) {
        this.setFocusableInTouchMode(true);
        this.setEGLContextClientVersion(2);
        this.setDebugFlags(DEBUG_CHECK_GL_ERROR | DEBUG_LOG_GL_CALLS);
        this.setRenderer(new GameGLES2Render(context));
    }
    
    private int     mPointerListCount       = 10;
    private int[]   mPointerIdList          = new int[mPointerListCount];
    private int[]   mPointerToolTypeList    = new int[mPointerListCount];
    private float[] mPointerXList           = new float[mPointerListCount];
    private float[] mPointerYList           = new float[mPointerListCount];
    
    // Return the pointer count of the event.
    private int getPointerDataList(MotionEvent event) {
        int count = event.getPointerCount();
        // Make sure the list size is big enough.
        if (mPointerListCount < count) {
            mPointerListCount       = count;
            mPointerIdList          = new int[mPointerListCount];
            mPointerToolTypeList    = new int[mPointerListCount];
            mPointerXList           = new float[mPointerListCount];
            mPointerYList           = new float[mPointerListCount];
        }

        for (int i = 0; i < count; ++i) {
            mPointerIdList[i]       = event.getPointerId(i);
            mPointerToolTypeList[i] = (VERSION.SDK_INT >= 14) ? 
                    event.getToolType(i): 0; 
            mPointerXList[i] = event.getX(i);
            mPointerYList[i] = event.getY(i);
        }
        return count;
    }
    
    public boolean onTouchEvent(MotionEvent event)
    {
        boolean ret = false;
        int     action = event.getActionMasked();
        switch (action) {
        // touch actions
        case MotionEvent.ACTION_CANCEL:
        case MotionEvent.ACTION_DOWN:
        case MotionEvent.ACTION_MOVE:
        case MotionEvent.ACTION_POINTER_DOWN:
        case MotionEvent.ACTION_POINTER_UP:
        case MotionEvent.ACTION_UP:
            //logTouchEvent(event);
            int source = (VERSION.SDK_INT >= 9) ? event.getSource() : 0;
            ret = NativeWrapper.MotionEvent.onPointerAction(
                    action,
                    source,
                    getPointerDataList(event), 
                    mPointerIdList,
                    mPointerToolTypeList,
                    mPointerXList,
                    mPointerYList);
            break;
        default:
            ret = super.onTouchEvent(event);
        }
        return ret;
    }
    
    public void logTouchEvent(MotionEvent event){
        StringBuffer msg = new StringBuffer("onTouchEvent");
        msg.append("( action: ").append(event.getActionMasked());
        int source = (VERSION.SDK_INT >= 9) ? event.getSource() : 0;
        msg.append(", source: ").append(source).append(", { ");
        for (int i = 0; i < event.getPointerCount(); ++i) {
            msg.append(event.getPointerId(i)).append(", ");
            msg.append((VERSION.SDK_INT >= 14) ? event.getToolType(i) : 0)
                .append(", ");
            msg.append(event.getX(i)).append(", ");
            msg.append(event.getY(i)).append(", ");
        }
        msg.append(" } )");
        Log.d("GameView", msg.toString());
    }
}
