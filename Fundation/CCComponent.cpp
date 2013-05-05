#include "pch.h"

#include "CCComponent.h"

#include <typeinfo>

using namespace std;

#define IMPLEMENT_CLASS     CCComponent

CCComponent::CCComponent()
    : _owner(nullptr)
{
}

CCComponent::~CCComponent()
{
}

CCString CCComponent::getName()
{
    return typeid(*this).name();
}
