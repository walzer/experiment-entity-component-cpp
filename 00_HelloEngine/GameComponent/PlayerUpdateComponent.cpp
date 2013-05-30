#include "pch.h"

#include "PlayerUpdateComponent.h"
#if (DEMO_NO_CALL_FUNC)
#include "MoveComponent.h"
#endif
using namespace std;

#define IMPLEMENT_CLASS PlayerUpdateComponent
CCCOMPONENT_REGISTER_CREATOR;

bool PlayerUpdateComponent::init(CCContext *context)
{
    BaseType::init(context);
    _onPointerDownEventHandler = context->getSurface()->onPointerDownEvent.add(
        bind(&PlayerUpdateComponent::onPointerDown, this, placeholders::_1));
#if (DEMO_NO_CALL_FUNC)
    _moveComponent = getOwner()->findComponent("class MoveComponent")->as<MoveComponent>();
#else
    _setDestPosition = getOwner()->findComponent("class MoveComponent")->findFunction<void (const CCVec3 &)>("setDestPosition");
#endif
    return true;
}

void PlayerUpdateComponent::done()
{
    getOwner()->getContext()->getSurface()->onPointerDownEvent.remove(
        _onPointerDownEventHandler);
}

void PlayerUpdateComponent::update(float dt)
{
}

bool PlayerUpdateComponent::onPointerDown(const PointerArgs &pointer)
{
    CCLOGI("PlayerUpdateComponent::onPointerDown(%f, %f)\n", pointer.x, pointer.y);
    CCVec3 dest = { pointer.x, pointer.y, 0};
#if (DEMO_NO_CALL_FUNC)
    _moveComponent->setDestPosition(dest);
#else
    _setDestPosition(cref(dest));
#endif
    return true;
}

#undef IMPLEMENT_CLASS
