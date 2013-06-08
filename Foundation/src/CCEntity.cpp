#include "pch.h"

#include "CCEntity.h"

using namespace std;

#define IMPL_CLASS     CCEntity

bool CCEntity::init()
{
    BaseType::init(_context);
    for_each(_components.begin(), _components.end(), [this](const pair<String, CCComponent::Ptr> &pair)
    {
            pair.second->init(_context);
    });
    return true;
}

void CCEntity::done()
{
}

void IMPL_FUNCTION(update, float dt)

}

void IMPL_FUNCTION(addComponent, const CCComponent::Ptr& com)
    _components.insert(make_pair(com->getName(), com));
    com->setOwner(this->asShared<CCEntity>());
}

CCComponent::Ptr IMPL_FUNCTION(findComponent, const String& name)
    auto it = _components.find(name);
    return (it != _components.end()) ? it->second : nullptr;
}

void IMPL_FUNCTION(removeComponent, const String& name)
    _components.erase(name);
}

void IMPL_FUNCTION(addChild, const Ptr& child)
    _children.push_back(child);
    child->setParent(static_pointer_cast<ThisType>(shared_from_this()));
}

const CCEntity::Ptr CCEntity::NullPtr;

const CCEntity::Ptr& IMPL_FUNCTION(findChild, const String& name)
    auto iter = find_if(_children.begin(), _children.end(), 
    [&name](const CCEntity::Ptr& ptr)->bool
    {
        return name == ptr->getName();
    });
    return (iter != _children.end()) ? *iter : NullPtr;
}

void IMPL_FUNCTION(removeChild, const Ptr& child)
    auto iter = find(_children.begin(), _children.end(), child);
    _children.erase(iter);
}

void IMPL_FUNCTION(setParent, const Ptr& parent)
    _parent = parent;
}