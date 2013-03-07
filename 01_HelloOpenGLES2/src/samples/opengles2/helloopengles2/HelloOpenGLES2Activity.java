package samples.opengles2.helloopengles2;

import samples.fundation.GameView;
import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;

public class HelloOpenGLES2Activity extends Activity {

    static {
        System.loadLibrary("HelloOpenGLES2");
    }
    
	private GameView mView;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		// Create a GLSurfaceView instance and set it
        // as the ContentView for this Activity.
		mView = new GameView(this);
        setContentView(mView);
	}

    @Override
    protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mView.onResume();
    }
    
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.hello_open_gles2, menu);
		return true;
	}

}
