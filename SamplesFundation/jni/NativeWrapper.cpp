#include "NativeWrapper.h"

#include "Debug.h"

static const char * kClassName="samples/fundation/NativeWrapper";
static struct
{
    jmethodID readDataFromAsset;
} s_method;

static JNINativeMethod kJavaMethods[] =
{
    {"readDataFromAsset", "(Ljava/lang/String;)[B", &s_method.readDataFromAsset},
};
static const int kNumJavaMethods = sizeof(kJavaMethods) / sizeof(JNINativeMethod);

bool NativeWrapper::init()
{
    static bool s_inited = false;
    if (s_inited) return true;

    int findNum = JNIHelper::findClassMethods(
            JNIHelper::getJNIEnv(),
            kClassName,
            kJavaMethods,
            kNumJavaMethods);
    s_inited = (findNum == kNumJavaMethods) ? true : false;

    return s_inited;
}

jbyteArray NativeWrapper::readDataFormAsset(const char * path)
{
    jbyteArray ret = 0;
    JNIEnv * env = JNIHelper::getJNIEnv();
    do
    {
        if (!path || !env) break;
        jstring jPath = env->NewStringUTF(path);
        if (!jPath) break;
        ret = (jbyteArray)env->CallStaticObjectMethod(
                env->FindClass(kClassName),
                s_method.readDataFromAsset,
                jPath);
        env->DeleteLocalRef(jPath);
    } while (false);
    return ret;
}
