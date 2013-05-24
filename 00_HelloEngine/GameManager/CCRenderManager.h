#ifndef __MANAGER__CCRENDER_MANAGER_H__
#define __MANAGER__CCRENDER_MANAGER_H__

#include "CCFoundation.h"

class CCRenderManager : public CCManager
{
public:
    typedef CCRenderManager ThisType;
    typedef CCManager BaseType;

    virtual ~CCRenderManager();

    virtual bool init(CCContext *context);
    virtual void done(CCContext *context);

protected:
    void begin();
    void end();
    CCDelegateHandler beginDelegate;
    CCDelegateHandler endDelegate;
};

#endif  // __MANAGER__CCRENDER_MANAGER_H__
