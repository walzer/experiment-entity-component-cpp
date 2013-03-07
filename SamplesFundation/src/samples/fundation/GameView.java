package samples.fundation;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

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
        this.setRenderer(new GameGLES2Render(context));
    }
}
