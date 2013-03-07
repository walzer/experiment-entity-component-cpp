#include "Runtime.h"

#include "Debug.h"
#include "Surface.h"

static RuntimeListener* s_listener;

RuntimeListener::RuntimeListener()
{
    s_listener = this;
}

RuntimeListener::~RuntimeListener()
{
    s_listener = nullptr;
}

static Surface * s_surface;

int Runtime::addSurface(Surface* surface)
{
    s_surface = surface;
    s_listener->onSurfaceCreated(surface);
    return 0;
}

Surface * Runtime::getSurface(int id)
{
    return s_surface;
}
