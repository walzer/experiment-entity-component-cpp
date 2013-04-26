#pragma once

#include <map>
#include <memory>
#include <vector>

#include "CCString.h"

class CCManager;

class CCContext
{
public:
    typedef CCContext                       ThisType;
    typedef ::std::shared_ptr<ThisType>     Ptr;
    typedef ::std::shared_ptr<CCManager>    ManagerPtr;

    virtual ~CCContext();

    virtual bool init();
    virtual void done();

    ThisType&   add(ManagerPtr manager, const CCString& name);
    CCManager*  get(const CCString& name);

    void run();
private:
    ::std::map<CCString, CCManager*> _nameMap;
    ::std::vector<::std::shared_ptr<CCManager>> _managers;
};