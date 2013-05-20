#ifndef __FOUNDATION__CCENTITY_MANAGER_H__
#define __FOUNDATION__CCENTITY_MANAGER_H__

#include <vector>

#include "CCManager.h"
#include "CCString.h"

class CCEntity;
class CCContext;

class CCEntityManager : public CCManager
{
public:
    typedef ::std::shared_ptr<CCEntity> EntityPtr;

    CCEntityManager():_context(nullptr){}
    virtual ~CCEntityManager();

    virtual bool init(CCContext * ctx);
    virtual void done(CCContext * ctx);

    void add(const EntityPtr& entity);
    const EntityPtr& find(const CCString& name);
    void remove(const EntityPtr& entity);

    void update(CCContext * context, float dt);

public:
    //static EntityPtr create(const CCString& name)
    //{
    //    return ::std::static_pointer_cast<CCEntity>(CCComponent::create(name));
    //}

private:
    ::std::vector<EntityPtr> _entities;
    CCContext* _context;
};

#endif  // __FOUNDATION__CCENTITY_MANAGER_H__
