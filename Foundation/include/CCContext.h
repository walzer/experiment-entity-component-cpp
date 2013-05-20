#ifndef __FOUNDATION__CCCONTEXT_H__
#define __FOUNDATION__CCCONTEXT_H__

#include <map>
#include <memory>
#include <vector>

#include "CCEvent.h"
#include "CCString.h"

class CCManager;
class CCTimeManager;

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

    ThisType&   setTimeManager(::std::shared_ptr<CCTimeManager> tm);
    CCTimeManager* getTimeManager();

    CCEvent<void (CCContext*, float)> updateEvent;

    void run();
private:
    ::std::shared_ptr<CCTimeManager> _tm;
    ::std::map<CCString, CCManager*> _nameMap;
    ::std::vector<::std::shared_ptr<CCManager>> _managers;
};

#endif  // __FOUNDATION__CCCONTEXT_H__
