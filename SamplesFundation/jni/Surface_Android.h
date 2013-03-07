#ifndef SURFACE_ANDROID_H_
#define SURFACE_ANDROID_H_

#include "Surface.h"

class Surface_Android: public Surface
{
public:
    Surface_Android();
    virtual ~Surface_Android();

    void setSize(int w, int h);
};

#endif /* SURFACE_ANDROID_H_ */
