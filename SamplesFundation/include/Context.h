#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Surface.h"

class Context
{
public:
    virtual ~Context();

    Surface* getSurface() { return mSurface; }
    void setSurface(Surface * surface) { mSurface = surface; }

    virtual bool init() = 0;
    virtual int run() = 0;

private:
    Surface * mSurface;
};

#endif /* __CONTEXT_H__ */
