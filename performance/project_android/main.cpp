#include "pch.h"

#include "Performace.h"
#include "profile.h"

extern "C"
{
    jint JNI_OnLoad(JavaVM *vm, void *reserved)
    {
        ProfileInit();
        profileFunction();
        ProfileUnInit();

        return JNI_VERSION_1_4;
    }
}
