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
        placeholders::_1, 
        placeholders::_2));
    return true;
}

void IMPLEMENT_FUNCTION(done, CCContext * ctx)
    ctx->updateEvent.remove(updateHandler);
    _entities.clear();
}

void IMPLEMENT_FUNCTION(update, CCContext* ctx, float dt)
    for_each(_entities.begin(), _entities.end(),
    [dt](const shared_ptr<CCEntity>& ptr)
    {
        ptr->update(dt);
    });
}
