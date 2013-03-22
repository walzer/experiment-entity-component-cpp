#include "Runtime_Android.h"

#include "Debug.h"

#include "NativeWrapper.h"
#include "SampleTools.h"
#include "Surface_Android.h"
#include "OGLES2Tools.h"

void Runtime_Android::createSurface()
{
    static Surface_Android surface;
    Runtime::addSurface(&surface);
}

bool Runtime_Android::init()
{
    bool ret = false;
    do
    {
        if (false == NativeWrapper::init()) break;
        CPVRTResourceFile::SetLoadReleaseFunctions(
                (void*)readDataFromAsset,
                (void*)releaseDataFromAsset);
        ret = true;
    } while (false);
    return ret;
}
