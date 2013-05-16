#include "pch.h"

#include "CCEntity.h"

using namespace std;

#define IMPLEMENT_CLASS     CCEntity

void IMPLEMENT_FUNCTION(update, float dt)

}

void IMPLEMENT_FUNCTION(addComponent, const CCComponent::Ptr& com)
    _components.insert(make_pair(com->getName(), com));
}

CCComponent::Ptr IMPLEMENT_FUNCTION(findComponent, const CCString& name)
    auto it = _components.find(name);
    return (it != _components.end()) ? it->second : nullptr;
}

void IMPLEMENT_FUNCTION(removeComponent, const CCString& name)
    _components.erase(name);
}

void IMPLEMENT_FUNCTION(addChild, const Ptr& child)
    _children.push_back(child);
    child->setParent(static_pointer_cast<ThisType>(shared_from_this()));
}

const CCEntity::Ptr CCEntity::NullPtr;

const CCEntity::Ptr& IMPLEMENT_FUNCTION(findChild, const CCString& name)
    auto iter = find_if(_children.begin(), _children.end(), 
    [&name](const CCEntity::Ptr& ptr)->bool
    {
        return name == ptr->getName();
    });
    return (iter != _children.end()) ? *iter : NullPtr;
}

void IMPLEMENT_FUNCTION(removeChild, const Ptr& child)
    auto iter = find(_children.begin(), _children.end(), child);
    _children.erase(iter);
}

void IMPLEMENT_FUNCTION(setParent, const Ptr& parent)
    _parent = parent;
}