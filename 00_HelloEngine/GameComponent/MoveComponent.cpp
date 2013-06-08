#include "pch.h"

#include "MoveComponent.h"

using namespace std;

#define IMPL_CLASS MoveComponent
CCCOMPONENT_REGISTER_CREATOR;

bool MoveComponent::init(CCContext *context)
{
    _destPosition.array.fill(0);
    _moving = false;
    BaseType::init(context);
    CCCOMPONENT_REGISTER_MEMBER_FUNCTION(setDestPosition, void, const CCVec3 &);

    auto transformCom = getOwner()->findComponent("class CCTransformComponent");
    _getPosition = transformCom->findFunction<const CCVec3 &()>("getPosition");
    _setPosition = transformCom->findFunction<void (const CCVec3&)>("setPosition");
    _updateEventHandler = context->updateEvent.add(bind(&MoveComponent::doMove, this, placeholders::_1));
    return true;
}

void MoveComponent::done()
{
    getOwner()->getContext()->updateEvent.remove(_updateEventHandler);
    unregisterFunction("setDestPosition");
}

void MoveComponent::setDestPosition(const CCVec3 &dest)
{
    _destPosition = dest;
    auto position = _getPosition();
    float distanceX = _destPosition.x - position.x;
    float distanceY = _destPosition.y - position.y;
    float value = 0.01f;
    _step.x = (distanceX > value) ? value : (distanceX < -value) ? -value : distanceX;
    _step.y = (distanceY > value) ? value : (distanceY < -value) ? -value : distanceY;
    _moving = true;
}

void MoveComponent::doMove(float dt)
{
    if (!_moving)
    {
        return;
    }
    auto position = _getPosition();
    float distanceX = _destPosition.x - position.x;
    float distanceY = _destPosition.y - position.y;
    if (::fabs(distanceX) <= ::fabs(_step.x))
    {
        _step.x = distanceX;
    }
    if (::fabs(distanceY) <= ::fabs(_step.y))
    {
        _step.y = distanceY;
    }
    CC_LOGI("step(%f, %f)\n", _step.x, _step.y);
    position.x += _step.x;
    position.y += _step.y;
    _setPosition(position);
    if (_step.x == 0 && _step.y == 0)
    {
        _moving = false;
    }
}

#undef IMPL_CLASS
