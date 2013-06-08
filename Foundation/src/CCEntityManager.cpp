#include "pch.h"

#include "CCEntityManager.h"

#include "CCContext.h"
#include "CCEntity.h"

using namespace std;

#define IMPL_CLASS     CCEntityManager

IMPL_FUNCTION(~CCEntityManager)

}

CCDelegateHandler updateHandler;
bool IMPL_FUNCTION(init, CCContext * ctx)
    _context = ctx;
    updateHandler = ctx->updateEvent.add(bind(
        &CCEntityManager::update, 
        this, 
        placeholders::_1));
    return true;
}

void IMPL_FUNCTION(done, CCContext * ctx)
    ctx->updateEvent.remove(updateHandler);
    _entities.clear();
}

void IMPL_FUNCTION(update, float dt)
    for_each(_entities.begin(), _entities.end(),
    [dt](const shared_ptr<CCEntity>& ptr)
    {
        ptr->update(dt);
    });
}

void IMPL_FUNCTION(add, const EntityPtr& entity)
    _entities.push_back(entity);
    entity->setContext(_context);
    entity->init();
}

const CCEntityManager::EntityPtr& IMPL_FUNCTION(find, const String& name)
    for (auto it = _entities.begin(); it != _entities.end(); ++it)
    {
        if ((*it)->getName() == name)
        {
            return *it;
        }
    }
    return CCEntity::NullPtr;
}

void IMPL_FUNCTION(remove, const CCEntityManager::EntityPtr& entity)
    for (auto it = _entities.begin(); it != _entities.end(); ++it)
    {
        if ((*it) == entity)
        {
            _entities.erase(it);
            break;
        }
    }
}
