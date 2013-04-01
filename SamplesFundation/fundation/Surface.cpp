#include "Surface.h"

#include "Context.h"

Surface::Surface()
{

}

Surface::~Surface()
{

}

Context * Surface::getContext()
{
    return mContext;
}

void Surface::setContext(Context * context)
{
    mContext = context;
    mContext->setSurface(this);
}

void Surface::onEvents()
{
    PointerData args;
    while(pointerQueue.read(args))
    {
        pointerEvent.raise(this, args);
    }
}

int Surface::getWidth()
{
    return mWidth;
}

int Surface::getHeight()
{
    return mHeight;
}
