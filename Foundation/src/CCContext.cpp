#include "pch.h"

#include "CCContext.h"

#include "CCManager.h"
#include "CCSurface.h"
#include "CCTimeManager.h"

using namespace std;

#define IMPL_CLASS     CCContext

CCContext::CCContext()
{
}

IMPL_FUNCTION(~CCContext)

}

bool IMPL_FUNCTION(init)
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

void IMPL_FUNCTION(done)
   for_each(_managers.begin(), _managers.end(), [this](ManagerPtr& manager)
    {      
        manager->done(this);
    });
    _managers.clear();
    _tm->done(this);
    _tm.reset();
}

CCContext& IMPL_FUNCTION(add, ManagerPtr manager, const String& name)
    _nameMap.insert(make_pair(name, manager.get()));
    _managers.push_back(manager);
    return *this;
}

CCManager* IMPL_FUNCTION(get, const String& name)
    auto iter = _nameMap.find(name);
    return (iter != _nameMap.end()) ? iter->second : nullptr;
}

CCContext& IMPL_FUNCTION(setTimeManager, shared_ptr<CCTimeManager> tm)
    _tm = tm;
    return *this;
}

CCTimeManager* IMPL_FUNCTION(getTimeManager)
    return _tm.get();
}

void IMPL_FUNCTION(run)
    float dt = _tm->getDeltaTime();
    updateEvent(dt);
    preDrawEvent();
    drawEvent();
    postDrawEvent();
    _surface->swapBuffer();
}
