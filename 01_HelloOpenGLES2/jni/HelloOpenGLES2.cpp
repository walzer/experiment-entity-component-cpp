#include <jni.h>

#include "HelloContext.h"

#include "Runtime.h"

class Application : public RuntimeListener
{
public:
    virtual void onSurfaceCreated(Surface * surface)
    {
        surface->setContext(new HelloContext());
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
