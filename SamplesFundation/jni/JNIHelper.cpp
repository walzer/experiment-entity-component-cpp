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

jclass JNIHelper::findClass(JNIEnv * env, const char * className)
{
    jclass clazz = nullptr;
    if (env && className)
    {
        clazz = env->FindClass(className);
        if (!clazz)
        {
            Debug::loge("(%04d)%s(%s):failed",
                    __LINE__,
                    __FUNCTION__,
                    className);
        }
    }
    return clazz;
}

bool JNIHelper::registerNativeMethods(
            JNIEnv * env,
            const char * className,
            const JNINativeMethod * methods,
            int numMethods)
{
    bool ret = false;
    do
    {
        if (!env || !className || !methods || numMethods <= 0)
        {
            Debug::loge("(%04d)%s:Invalid agrs!", __LINE__, __FUNCTION__);
            break;
        }
        jclass clazz = env->FindClass(className);
        if (!clazz)
        {
            Debug::loge("(%04d)%s:Can't find class(%s)",
                    __LINE__,
                    __FUNCTION__,
                    className);
            break;
        }
        int rtn = env->RegisterNatives(clazz, methods, numMethods);
        if (rtn != JNI_OK)
        {
            Debug::logd("RegisterNatives failed for(%s), return(%d)!",
                    className,
                    rtn);
            break;
        }
        ret = true;
    } while (false);
    return ret;
}

bool JNIHelper::findClassMethods(
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
            Debug::loge("(%04d)%s:Invalid agrs!", __LINE__, __FUNCTION__);
            break;
        }
        jclass clazz = env->FindClass(className);
        if (!clazz)
        {
            Debug::loge("(%04d)%s:Can't find class(%s)",
                    __LINE__,
                    __FUNCTION__,
                    className);
            break;
        }
        jmethodID id;
        for(int i = 0; i < numMethods; ++i)
        {
            const JNINativeMethod& method = methods[i];
            id = env->GetStaticMethodID(clazz, method.name, method.signature);
            if (!id)
            {
                Debug::loge("(%04d)%s:GetStaticMethodID failed for(%s(%s))",
                    __LINE__,
                    __FUNCTION__,
                    method.name,
                    method.signature);
                continue;
            }
            *((jmethodID*)method.fnPtr) = id;
            ++ret;
        }
    } while (false);
    return (ret == numMethods) ? true : false;
}
