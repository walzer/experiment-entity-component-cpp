#include "pch.h"

#include "CCComponent.h"

#include <typeinfo>

using namespace std;

#define IMPLEMENT_CLASS     CCComponent

CCCOMPONENT_REGISTER_CREATOR;

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

CCComponent::Ptr CCComponent::create(const CCString& typeName)
{
    CCComponent::Ptr ret;
    auto it = getCreatorEntry().find(typeName);
    if (it != getCreatorEntry().end())
    {
        ret = it->second();
    }
    return ret;
}

CCComponent::CreatorEntry& CCComponent::getCreatorEntry()
{
    static CCComponent::CreatorEntry s_entry;
    return s_entry;
}

void CCComponent::registerCreator(const CCString& comName, const Creator& creator)
{
    getCreatorEntry().insert(make_pair(comName, creator));
}
