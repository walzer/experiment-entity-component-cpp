#ifndef __FUNDATION__CCENTITY_H__
#define __FUNDATION__CCENTITY_H__

#include <map>
#include <memory>

#include "CCComponent.h"
#include "CCString.h"

class CCEntity : public CCComponent
{
public:
    typedef CCEntity ThisType;

    virtual void update(float dt);

    ThisType& add(::std::shared_ptr<CCComponent>& com);
    CCComponent* get(CCString comName);
    ThisType& remove(CCString comName);

private:
    ::std::map<CCString, ::std::shared_ptr<CCComponent>> _components;
};

#endif  // __FUNDATION__CCENTITY_H__
