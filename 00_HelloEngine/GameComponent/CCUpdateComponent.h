#ifndef __FOUNDATION__CCUPDATE_COMPONENT_H__
#define __FOUNDATION__CCUPDATE_COMPONENT_H__

#include "CCComponent.h"

class CCUpdateComponent : public CCComponent
{
public:
    typedef CCComponent BaseType;
    typedef CCUpdateComponent ThisType;

    virtual bool init(CCContext* context);
    virtual void done();

protected:
    virtual void update(float dt) = 0;

    CCDelegateHandler updateEventHandler;
};

#endif  // __FOUNDATION__CCUPDATE_COMPONENT_H__
