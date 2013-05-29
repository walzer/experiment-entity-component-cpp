#ifndef __MOVE_COMPONENT_H__
#define __MOVE_COMPONENT_H__

#include "CCFoundation.h"

class MoveComponent : public CCComponent
{
public:
    typedef CCComponent BaseType;
    typedef MoveComponent ThisType;

    virtual bool init(CCContext *context);
    virtual void done();

protected:
    void doMove(float dt);

    CCVec3 _destPosition;
    CCDelegateHandler _updateEventHandler;
    ::std::function<const CCVec3& ()> _getPosition;
    ::std::function<void (const CCVec3&)> _setPosition;
};

#endif  // __MOVE_COMPONENT_H__
