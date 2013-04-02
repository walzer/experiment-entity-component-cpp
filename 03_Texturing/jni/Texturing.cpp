#include <jni.h>

#include "MipmapContext.h"
#include "MultiTextureContext.h"

#include "pal/android/JNIHelper.h"
#include "pal/android/Runtime_Android.h"

class Application : public RuntimeListener
{
public:
    virtual void onSurfaceCreated(Surface * surface)
    {
#if 0
        MipmapContext * context = new MipmapContext();
#else
        MultiTextureContext * context = new MultiTextureContext();
#endif
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
