#include "samples_fundation_NativeWrapper_GameGLES2Render.h"

#include "Context.h"
#include "Debug.h"
#include "Runtime_Android.h"
#include "Surface_Android.h"

JNIEXPORT void JNICALL Java_samples_fundation_NativeWrapper_00024GameGLES2Render_onDrawFrame
  (JNIEnv *, jclass)
{
    Runtime::getSurface(0)->getContext()->run();
}

JNIEXPORT void JNICALL Java_samples_fundation_NativeWrapper_00024GameGLES2Render_onSurfaceChanged
  (JNIEnv *, jclass, jint w, jint h)
{
    dynamic_cast<Surface_Android*>(Runtime::getSurface(0))->setSize(w, h);
}

JNIEXPORT void JNICALL Java_samples_fundation_NativeWrapper_00024GameGLES2Render_onSurfaceCreated
  (JNIEnv *, jclass)
{
    Runtime_Android::createSurface();
}
