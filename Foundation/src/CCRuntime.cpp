#include "pch.h"

#include "CCContext.h"
#include "CCRuntime.h"
#include "CCSurface.h"

CCRuntime * CCRuntime::s_instance = nullptr;

CCRuntime * CCRuntime::getInstance()
{
    return s_instance;
}

CCRuntime::CCRuntime()
    : _appInstance()
    , _surface()
{
    s_instance = this;
}

CCRuntime::~CCRuntime()
{
    s_instance = nullptr;
}

bool CCRuntime::init(void * appInstance)
{
    _appInstance = appInstance;
    return true;
}

void CCRuntime::done()
{
    _appInstance = nullptr;
    if (_surface)
    {
        _surface.reset();
    }
}

void * CCRuntime::getAppInstance()
{
    return _appInstance;
}

CCSurface * CCRuntime::getSurface()
{
    return _surface.get();
}

void CCRuntime::run()
{
    auto context = _surface->getContext();
    if (context)
    {
        context->run();
    }
}

void CCRuntime::addSurface(const ::std::shared_ptr<CCSurface>& surface)
{
    _surface = surface;
    onSurfaceCreated(surface.get());
}
