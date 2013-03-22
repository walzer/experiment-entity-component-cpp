#include <jni.h>

#include "AppContext.h"

#include "pal/android/JNIHelper.h"
#include "pal/android/Runtime_Android.h"

class Application : public RuntimeListener
{
public:
    virtual void onSurfaceCreated(Surface * surface)
    {
        AppContext * context = new AppContext();
        surface->setContext(context);
        context->init();
    }
};

extern "C"
{
    jint JNI_OnLoad(JavaVM *vm, void *reserved)
    {
        JNIHelper::s_vm = vm;
        Runtime_Android::init();

        static Application app;

        return JNI_VERSION_1_4;
    }
}
