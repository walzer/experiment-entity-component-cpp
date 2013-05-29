#ifndef __FOUNDATION__CCCONTEXT_H__
#define __FOUNDATION__CCCONTEXT_H__

#include <map>
#include <memory>
#include <vector>

#include "CCEvent.h"
#include "CCString.h"

class CCManager;
class CCSurface;
class CCTimeManager;

class CCContext
{
    // Uncopyable
    CCContext(const CCContext &);
    CCContext &operator = (const CCContext &);
public:
    typedef CCContext                       ThisType;
    typedef ::std::shared_ptr<ThisType>     Ptr;
    typedef ::std::shared_ptr<CCManager>    ManagerPtr;

    CCContext();
    virtual ~CCContext();

    virtual bool init();
    virtual void done();

    ThisType&   add(ManagerPtr manager, const CCString& name);
    CCManager*  get(const CCString& name);
    template <typename ManagerType>
    ManagerType *get(const CCString& name)
    {
        return static_cast<ManagerType *>(get(name));
    }
    ThisType&   setTimeManager(::std::shared_ptr<CCTimeManager> tm);
    CCTimeManager* getTimeManager();

    CCEvent<void (float)> updateEvent;
    CCEvent<void()> preDrawEvent;
    CCEvent<void()> drawEvent;
    CCEvent<void()> postDrawEvent;

    void setSurface(const ::std::shared_ptr<CCSurface>& surface)
    {
        _surface = surface;
    }
    const ::std::shared_ptr<CCSurface>& getSurface()
    {
        return _surface;
    }
    void run();
private:
    ::std::shared_ptr<CCSurface> _surface;
    ::std::shared_ptr<CCTimeManager> _tm;
    ::std::map<CCString, CCManager*> _nameMap;
    ::std::vector<::std::shared_ptr<CCManager>> _managers;
};

#endif  // __FOUNDATION__CCCONTEXT_H__
