#ifndef __PLAYER_UPDATE_COMPONENT_H__
#define __PLAYER_UPDATE_COMPONENT_H__

#include "CCSurface.h"

#include "CCUpdateComponent.h"

#define DEMO_NO_CALL_FUNC 1

#if (DEMO_NO_CALL_FUNC)
class MoveComponent;
#endif
class PlayerUpdateComponent : public CCUpdateComponent
{
public:
    typedef CCUpdateComponent BaseType;
    typedef PlayerUpdateComponent ThisType;

    virtual bool init(CCContext *context);
    virtual void done();

    void update(float dt);

protected:
    bool onPointerDown(const PointerArgs &pointer);

    CCDelegateHandler _onPointerDownEventHandler;
#if (DEMO_NO_CALL_FUNC)
    MoveComponent *_moveComponent;
#else
    ::std::function<void (const CCVec3 &)> _setDestPosition;
#endif
};

#endif  // __PLAYER_UPDATE_COMPONENT_H__
