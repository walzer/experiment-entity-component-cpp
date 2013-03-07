#ifndef RUNTIME_ANDROID_H_
#define RUNTIME_ANDROID_H_

#include "Runtime.h"

class Runtime_Android : public Runtime
{
public:
    static void createSurface();
};

#endif /* RUNTIME_ANDROID_H_ */
