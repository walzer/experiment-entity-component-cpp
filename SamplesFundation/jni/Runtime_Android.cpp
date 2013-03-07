#include "Runtime_Android.h"

#include "Debug.h"
#include "Surface_Android.h"

#include <jni.h>

void Runtime_Android::createSurface()
{
    static Surface_Android surface;
    Runtime::addSurface(&surface);
}
