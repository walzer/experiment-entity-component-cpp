#ifndef NATIVEWRAPPER_H_
#define NATIVEWRAPPER_H_

#include "JNIHelper.h"

struct NativeWrapper
{
    static bool init();

    static jbyteArray readDataFormAsset(const char * path);
};

#endif /* NATIVEWRAPPER_H_ */
