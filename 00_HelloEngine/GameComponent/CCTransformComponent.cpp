#include "pch.h"

#include "CCTransformComponent.h"

#define IMPLEMENT_CLASS CCTransformComponent
CCCOMPONENT_REGISTER_CREATOR;

bool CCTransformComponent::init(CCContext *context)
{
    _position.array.fill(0);
    BaseType::init(context);
    CCCOMPONENT_REGISTER_MEMBER_FUNCTION(getPosition, const CCVec3 &);
    CCCOMPONENT_REGISTER_MEMBER_FUNCTION(setPosition, void, const CCVec3 &pos);
    return true;
}

void CCTransformComponent::done()
{
    unregisterFunction("getPosition");
    unregisterFunction("setPosition");
}

#undef IMPLEMENT_CLASS
