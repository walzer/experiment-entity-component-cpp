#ifndef __FOUNDATION__CC_CONTEXT_H__
#define __FOUNDATION__CC_CONTEXT_H__

#include "CppStl.h"

#include "Event.h"
#include "String.h"

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

    ThisType&   add(ManagerPtr manager, const String& name);
    CCManager*  get(const String& name);
    template <typename ManagerType>
    ManagerType *get(const String& name)
    {
        return static_cast<ManagerType *>(get(name));
    }
    ThisType&   setTimeManager(::std::shared_ptr<CCTimeManager> tm);
    CCTimeManager* getTimeManager();

    Event<void (float)> updateEvent;
    Event<void()> preDrawEvent;
    Event<void()> drawEvent;
    Event<void()> postDrawEvent;

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
    ::std::map<String, CCManager*> _nameMap;
    ::std::vector<::std::shared_ptr<CCManager>> _managers;
};

#endif  // __FOUNDATION__CC_CONTEXT_H__
