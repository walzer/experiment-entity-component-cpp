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
}

int Surface::getWidth()
{
    return mWidth;
}

int Surface::getHeight()
{
    return mHeight;
}
