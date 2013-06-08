#ifndef __FOUNDATION__RUNTIME_H__
#define __FOUNDATION__RUNTIME_H__

#include <memory>

#include "Event.h"

class CCSurface;

class CCRuntime
{
    // Uncopyable
    CCRuntime(const CCRuntime &);
    CCRuntime &operator = (const CCRuntime &);
public:
    virtual ~CCRuntime();

    virtual bool init(void *appInstance);
    virtual void done();

    void *getAppInstance();
    CCSurface *getSurface();

    void run();

    static CCRuntime *getInstance();

    Event<void(CCSurface *)> onSurfaceCreated;

protected:
    CCRuntime();
    void addSurface(const ::std::shared_ptr<CCSurface> &surface);

    void *_appInstance;
    ::std::shared_ptr<CCSurface> _surface;

    static CCRuntime *s_instance;
};

#endif /* __FOUNDATION__RUNTIME_H__ */
