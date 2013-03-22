#include "pal/android/JNIHelper.h"

#include "Debug.h"

JavaVM * JNIHelper::s_vm;

JNIEnv * JNIHelper::getJNIEnv()
{
    JNIEnv * env = nullptr;
    if (s_vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
    {
        Debug::loge("Failed to obtain JNIEnv!");
    }
    return env;
}

int JNIHelper::registerNativeMethods(
            JNIEnv * env,
            const char * className,
            const JNINativeMethod * methods,
            int numMethods)
{
    Debug::logd("%d %s", __LINE__, __FUNCTION__);
    int ret = 0;
    do
    {
        if (!env || !className || !methods || numMethods <= 0)
        {
            Debug::logd("Invalid agrs!");
            break;
        }
        jclass clazz = env->FindClass(className);
        if (!clazz)
        {
            Debug::logd("Can't find class(%s)", className);
            break;
        }
        ret = env->RegisterNatives(clazz, methods, numMethods);
        if (ret < 0)
        {
            Debug::logd("RegisterNatives failed for(%s)!", className);
        }
    } while (false);
    Debug::logd("%d %s", __LINE__, __FUNCTION__);
    return ret;
}

int JNIHelper::findClassMethods(
        JNIEnv * env,
        const char * className,
        const JNINativeMethod * methods,
        int numMethods)
{
    int ret = 0;
    do
    {
        if (!env || !className || !methods || numMethods <= 0)
        {
            Debug::loge("Invalid agrs!");
            break;
        }
        jclass clazz = env->FindClass(className);
        if (!clazz)
        {
            Debug::loge("Can't find class(%s)", className);
            break;
        }
        jmethodID id;
        for(int i = 0; i < numMethods; ++i)
        {
            const JNINativeMethod& method = methods[i];
            id = env->GetStaticMethodID(clazz, method.name, method.signature);
            if (!id)
            {
                Debug::loge("RegisterNatives failed for(%s)!", className);
                continue;
            }
            *((jmethodID*)method.fnPtr) = id;
            ++ret;
        }
    } while (false);
    return ret;
}
