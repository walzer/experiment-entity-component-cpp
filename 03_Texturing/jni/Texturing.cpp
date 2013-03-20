#include <jni.h>

#include "AppContext.h"

#include "Runtime.h"

class Application : public RuntimeListener
{
public:
    virtual void onSurfaceCreated(Surface * surface)
    {
        AppContext * context = new AppContext();
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
