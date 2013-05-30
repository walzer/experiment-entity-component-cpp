#include "pch.h"

#include "CCRenderComponent.h"

#include "CCTransformComponent.h"
#include "GameManager/CCRenderManager.h"

#define IMPLEMENT_CLASS CCRenderComponent
CCCOMPONENT_REGISTER_CREATOR;

bool CCRenderComponent::init(CCContext* context)
{
    BaseType::init(context);
    _manager = context->get<CCRenderManager>("CCRenderManager");
    _transformComponent = getOwner()->findComponent("class CCTransformComponent")->as<CCTransformComponent>();
    drawEventHandler = context->drawEvent.add([this]()
    {
        this->_manager->addBatch(this, this->_transformComponent->getPosition());
    });
    return true;
}

void CCRenderComponent::done()
{
    getOwner()->getContext()->drawEvent.remove(drawEventHandler);
    _transformComponent = nullptr;
    _manager = nullptr;
    BaseType::done();
}

#undef IMPLEMENT_CLASS
