#include "pch.h"

#include "CCContext.h"

#include "CCManager.h"

using namespace std;

#define IMPLEMENT_CLASS     CCContext

IMPLEMENT_FUNCTION(~CCContext)

}

bool IMPLEMENT_FUNCTION(init)
    return true;
}

void IMPLEMENT_FUNCTION(done)

}

CCContext& IMPLEMENT_FUNCTION(add, ManagerPtr manager, const CCString& name)
    _nameMap.insert(make_pair(name, manager.get()));
    _managers.push_back(manager);
    return *this;
}

CCManager* IMPLEMENT_FUNCTION(get, const CCString& name)
    return nullptr;
}

void IMPLEMENT_FUNCTION(run)
    for_each(_managers.begin(), _managers.end(), [](ManagerPtr& manager) {      
        manager->update();
    });
}
