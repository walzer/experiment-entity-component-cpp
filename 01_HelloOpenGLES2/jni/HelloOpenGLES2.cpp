#include <jni.h>

#include "HelloContext2.h"

#include "pal/android/JNIHelper.h"
#include "pal/android/Runtime_Android.h"

class Application : public RuntimeListener
{
public:
    virtual void onSurfaceCreated(Surface * surface)
    {
        HelloContext * context = new HelloContext2();
        context->init();
        surface->setContext(context);
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
