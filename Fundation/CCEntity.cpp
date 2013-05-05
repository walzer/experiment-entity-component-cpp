#include "pch.h"

#include "CCEntity.h"

using namespace std;

#define IMPLEMENT_CLASS     CCEntity

void IMPLEMENT_FUNCTION(update, float dt)

}

CCEntity& IMPLEMENT_FUNCTION(add, shared_ptr<CCComponent>& com)
    _components.insert(make_pair(com->getName(), com));
    return *this;
}

CCComponent* IMPLEMENT_FUNCTION(get, CCString comName)
    auto it = _components.find(comName);
    return (it != _components.end()) ? it->second.get() : nullptr;
}

CCEntity& IMPLEMENT_FUNCTION(remove, CCString comName)
    _components.erase(comName);
    return *this;
}
