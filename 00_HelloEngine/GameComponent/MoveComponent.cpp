#include "pch.h"

#include "MoveComponent.h"

using namespace std;

#define IMPLEMENT_CLASS MoveComponent
CCCOMPONENT_REGISTER_CREATOR;

bool MoveComponent::init(CCContext *context)
{
    BaseType::init(context);
    auto transformCom = getOwner()->findComponent("class CCTransformComponent");
    _getPosition = transformCom->findFunction<const CCVec3 &()>("getPosition");
    _setPosition = transformCom->findFunction<void (const CCVec3&)>("setPosition");
    _updateEventHandler = context->updateEvent.add(bind(&MoveComponent::doMove, this, placeholders::_1));
    return true;
}

void MoveComponent::done()
{
}

void MoveComponent::doMove(float dt)
{
    auto position = _getPosition();
    float distanceX = _destPosition.x - position.x;
    float distanceY = _destPosition.y - position.y;
    float stepX = (distanceX > 0.02f) ? 0.02f : distanceX;
    float stepY = (distanceY > 0.02f) ? 0.02f : distanceY;
    position.x += stepX;
    position.y += stepY;
    _setPosition(position);
}

#undef IMPLEMENT_CLASS
