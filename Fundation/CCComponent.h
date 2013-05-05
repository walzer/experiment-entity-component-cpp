#ifndef __FUNDATION__CCCOMPONENT_H__
#define __FUNDATION__CCCOMPONENT_H__

#include "CCString.h"

class CCEntity;

class CCComponent
{
public:
    CCComponent();
    virtual ~CCComponent();

    virtual CCString getName();

private:
    CCEntity* _owner;
};

#endif  // __FUNDATION__CCCOMPONENT_H__
