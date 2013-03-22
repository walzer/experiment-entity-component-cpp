package samples.fundation;

import android.app.Application;

public class SampleApplication extends Application {
    private static SampleApplication s_instance;
    public SampleApplication() {
        s_instance = this;
    }

    public static SampleApplication getInstance() {
        return s_instance;
    }
}
