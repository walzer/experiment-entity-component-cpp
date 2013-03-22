#include "SampleTools.h"

//#include "Debug.h"

#include "NativeWrapper.h"

void* readDataFromAsset(const char* pPath, char** pData, long& size)
{
    char * ret = nullptr;
    do
    {
        if (!pPath || !pData) break;
        *pData = nullptr;

        jbyteArray jData = NativeWrapper::readDataFormAsset(pPath);
        if (!jData) break;

        JNIEnv* env = JNIHelper::getJNIEnv();
        size = env->GetArrayLength(jData);
        if (size)
        {
            *pData = new char[size];
            env->GetByteArrayRegion(jData, 0, size, (jbyte*)*pData);
        }
        env->DeleteLocalRef(jData);
        ret = *pData;
        //Debug::logd("%d %s(%s):%d", __LINE__, __FUNCTION__, pPath, size);
    } while (false);
    return ret;
}

bool  releaseDataFromAsset(void* handle)
{
    if(handle)
    {
        delete[] (char*) handle;
        return true;
    }

    return false;
}
