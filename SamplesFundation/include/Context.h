#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Surface.h"

class Context
{
public:
    Surface* getSurface();

    virtual int run() = 0;
};

#endif /* __CONTEXT_H__ */
