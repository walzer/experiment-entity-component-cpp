#include <jni.h>

#include "HelloContext2.h"

#include "Runtime.h"

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
        //JniHelper::setJavaVM(vm);

        static Application app;

        return JNI_VERSION_1_4;
    }
}
