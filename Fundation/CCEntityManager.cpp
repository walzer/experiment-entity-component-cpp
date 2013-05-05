#include "pch.h"

#include "CCEntityManager.h"

#include "CCContext.h"
#include "CCEntity.h"

using namespace std;

#define IMPLEMENT_CLASS     CCEntityManager

IMPLEMENT_FUNCTION(~CCEntityManager)

}

bool IMPLEMENT_FUNCTION(init, CCContext * ctx)
    _context = ctx;
    ctx->updateEvent.add(bind(
        &CCEntityManager::update, 
        this, 
        placeholders::_1, 
        placeholders::_2), "CCEntityManager::update");
    return true;
}

void IMPLEMENT_FUNCTION(done, CCContext * ctx)
    ctx->updateEvent.remove("CCEntityManager::update");
    _entities.clear();
}

void IMPLEMENT_FUNCTION(update, CCContext* ctx, float dt)
    for_each(_entities.begin(), _entities.end(),
    [dt](const shared_ptr<CCEntity>& ptr)
    {
        ptr->update(dt);
    });
}
