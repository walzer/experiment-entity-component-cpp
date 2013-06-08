#ifndef _CC__TO_STRINGRUNTIME_WIN32_H__
#define _CC__TO_STRINGRUNTIME_WIN32_H__

#include "CCFoundation.h"

class CCRuntime_win32 : public CCRuntime
{
public:
    typedef CCRuntime   BaseType;

    CCRuntime_win32();

    virtual bool init(void * appInstance);
};

#endif  // _CC__TO_STRINGRUNTIME_WIN32_H__
