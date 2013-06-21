#include "pch.h"

#include <jni.h>

#include "Event.h"
#include "Event2.h"
#include "profile.h"

using namespace cc;

extern "C"
{
    jint JNI_OnLoad(JavaVM *vm, void *reserved)
    {
        ProfileInit();
        eventTest();
        event2Test();
        ProfileUnInit();

        return JNI_VERSION_1_4;
    }
}
