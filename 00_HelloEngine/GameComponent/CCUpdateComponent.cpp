#include "pch.h"

#include "CCUpdateComponent.h"

//#define IMPL_CLASS CCUpdateComponent
using namespace std;

bool CCUpdateComponent::init(CCContext* context)
{
    BaseType::init(context);

    updateEventHandler = getOwner()->getContext()->updateEvent.add(
        bind(&CCUpdateComponent::update, this, placeholders::_1));
    return true;
}

void CCUpdateComponent::done()
{
    getOwner()->getContext()->updateEvent.remove(updateEventHandler);
    BaseType::done();
}
