#include "pch.h"

#include "CCContext.h"

#include "CCManager.h"
#include "CCTimeManager.h"

using namespace std;

#define IMPLEMENT_CLASS     CCContext

IMPLEMENT_FUNCTION(~CCContext)

}

bool IMPLEMENT_FUNCTION(init)
    bool ret = false;
    do
    {
        if (nullptr == _tm)
        {
            _tm.reset(new CCTimeManager);
        }
        if (! _tm->init(this)) break;

        auto it = _managers.begin();
        auto end = _managers.end();
        for (; it != end; ++it)
        {
            if (! (*it)->init(this)) break;
        }
        if (it != end) break;
        ret = true;
    } while (false);
    return true;
}

void IMPLEMENT_FUNCTION(done)
   for_each(_managers.begin(), _managers.end(), [this](ManagerPtr& manager)
    {      
        manager->done(this);
    });
    _managers.clear();
    _tm->done(this);
    _tm.reset();
}

CCContext& IMPLEMENT_FUNCTION(add, ManagerPtr manager, const CCString& name)
    _nameMap.insert(make_pair(name, manager.get()));
    _managers.push_back(manager);
    return *this;
}

CCManager* IMPLEMENT_FUNCTION(get, const CCString& name)
    return nullptr;
}

CCContext& IMPLEMENT_FUNCTION(setTimeManager, shared_ptr<CCTimeManager> tm)
    _tm = tm;
    return *this;
}

CCTimeManager* IMPLEMENT_FUNCTION(getTimeManager)
    return _tm.get();
}

void IMPLEMENT_FUNCTION(run)
    float dt = _tm->getDeltaTime();
    updateEvent.raise(this, dt);
}
