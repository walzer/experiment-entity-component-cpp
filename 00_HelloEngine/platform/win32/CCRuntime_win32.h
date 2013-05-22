#ifndef __CCRUNTIME_WIN32_H__
#define __CCRUNTIME_WIN32_H__

#include "CCFoundation.h"

class CCRuntime_win32 : public CCRuntime
{
public:
    typedef CCRuntime   BaseType;

    CCRuntime_win32();

    virtual bool init(void * appInstance);
};

#endif  // __CCRUNTIME_WIN32_H__
