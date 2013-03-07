#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Surface.h"

class Context
{
public:
    virtual ~Context();

    Surface* getSurface();

    virtual bool init() = 0;
    virtual int run() = 0;
};

#endif /* __CONTEXT_H__ */
