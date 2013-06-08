#ifndef __FOUNDATION_CC__TO_STRINGENTITY_MANAGER_H__
#define __FOUNDATION_CC__TO_STRINGENTITY_MANAGER_H__

#include <vector>

#include "CCManager.h"
#include "String.h"

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
    const EntityPtr& find(const String& name);
    void remove(const EntityPtr& entity);

    void update(float dt);

public:
    //static EntityPtr create(const String& name)
    //{
    //    return ::std::static_pointer_cast<CCEntity>(CCComponent::create(name));
    //}

private:
    ::std::vector<EntityPtr> _entities;
    CCContext* _context;
};

#endif  // __FOUNDATION_CC__TO_STRINGENTITY_MANAGER_H__
