#include "pch.h"

#include "CCEntityManager.h"

#include "CCContext.h"
#include "CCEntity.h"

using namespace std;

#define IMPLEMENT_CLASS     CCEntityManager

IMPLEMENT_FUNCTION(~CCEntityManager)

}

CCDelegateHandler updateHandler;
bool IMPLEMENT_FUNCTION(init, CCContext * ctx)
    _context = ctx;
    updateHandler = ctx->updateEvent.add(bind(
        &CCEntityManager::update, 
        this, 
        placeholders::_1));
    return true;
}

void IMPLEMENT_FUNCTION(done, CCContext * ctx)
    ctx->updateEvent.remove(updateHandler);
    _entities.clear();
}

void IMPLEMENT_FUNCTION(update, float dt)
    for_each(_entities.begin(), _entities.end(),
    [dt](const shared_ptr<CCEntity>& ptr)
    {
        ptr->update(dt);
    });
}

void IMPLEMENT_FUNCTION(add, const EntityPtr& entity)
    _entities.push_back(entity);
    entity->setContext(_context);
    entity->init();
}

const CCEntityManager::EntityPtr& IMPLEMENT_FUNCTION(find, const CCString& name)
    for (auto it = _entities.begin(); it != _entities.end(); ++it)
    {
        if ((*it)->getName() == name)
        {
            return *it;
        }
    }
    return CCEntity::NullPtr;
}

void IMPLEMENT_FUNCTION(remove, const CCEntityManager::EntityPtr& entity)
    for (auto it = _entities.begin(); it != _entities.end(); ++it)
    {
        if ((*it) == entity)
        {
            _entities.erase(it);
            break;
        }
    }
}
