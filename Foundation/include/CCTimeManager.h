#ifndef __FOUNDATION__CCTIME_MANAGER_H__
#define __FOUNDATION__CCTIME_MANAGER_H__

#include "CCManager.h"

class CCTimeManager : public CCManager
{
public:
    virtual bool init(CCContext *){ return true; }
    virtual void done(CCContext *){}

    float getDeltaTime();
};

#endif  // __FOUNDATION__CCTIME_MANAGER_H__
