#ifndef __FUNDATION__CCENTITY_MANAGER_H__
#define __FUNDATION__CCENTITY_MANAGER_H__

#include "CCManager.h"

#include <vector>

class CCEntity;
class CCContext;

class CCEntityManager : public CCManager
{
public:
    CCEntityManager():_context(nullptr){}
    virtual ~CCEntityManager();

    virtual bool init(CCContext * ctx);
    virtual void done(CCContext * ctx);

    void update(CCContext * context, float dt);

private:
    ::std::vector<::std::shared_ptr<CCEntity>> _entities;
    CCContext* _context;
};

#endif  // __FUNDATION__CCENTITY_MANAGER_H__
