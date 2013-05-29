#include "pch.h"

#include "CCRenderComponent.h"

#include "GameManager/CCRenderManager.h"

#define IMPLEMENT_CLASS CCRenderComponent
CCCOMPONENT_REGISTER_CREATOR;

bool CCRenderComponent::init(CCContext* context)
{
    BaseType::init(context);
    _manager = context->get<CCRenderManager>("CCRenderManager");
    drawEventHandler = context->drawEvent.add([this]()
    {
        this->_manager->addBatch(this);
    });
    return true;
}

void CCRenderComponent::done()
{
    getOwner()->getContext()->drawEvent.remove(drawEventHandler);
    BaseType::done();
}

#undef IMPLEMENT_CLASS
