#include "pch.h"

#include "CCSurface.h"

#include "CCContext.h"

CCSurface::CCSurface()
    : _context()
    , _width()
    , _height()
{

}

CCSurface::~CCSurface()
{

}

bool CCSurface::init()
{
    return true;
}

void CCSurface::done()
{
    if (_context)
    {
        _context->done();
        _context.reset();
    }
}

CCContext * CCSurface::getContext()
{
    return _context.get();
}

void CCSurface::setContext(const ::std::shared_ptr<CCContext>& context)
{
    _context = context;
    _context->setSurface(shared_from_this());
    _context->init();
}

int CCSurface::getWidth()
{
    return _width;
}

int CCSurface::getHeight()
{
    return _height;
}
