#ifndef __FOUNDATION_CC__TO_STRINGTIME_MANAGER_H__
#define __FOUNDATION_CC__TO_STRINGTIME_MANAGER_H__

#include "CCManager.h"

class CCTimeManager : public CCManager
{
public:
    virtual bool init(CCContext *){ return true; }
    virtual void done(CCContext *){}

    float getDeltaTime();
};

#endif  // __FOUNDATION_CC__TO_STRINGTIME_MANAGER_H__
