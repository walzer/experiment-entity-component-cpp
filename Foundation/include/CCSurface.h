#ifndef __CCSURFACE_H__
#define __CCSURFACE_H__

#include "CCEvent.h"

class CCContext;

struct PointerArgs
{
    int id;
    float x;
    float y;
};

typedef enum
{
    ACTION_UNKNOWN = 0,
    ACTION_DOWN,
    ACTION_UP,
    ACTION_MOVE,
    ACTION_CANCEL,
} ActionType;

static const int MAX_POINTER_COUNT = 10;

struct PointerData
{
    ActionType      action;
    int             count;
    PointerArgs     args[MAX_POINTER_COUNT];
};

class CCSurface : public ::std::enable_shared_from_this<CCSurface>
{
public:
    virtual ~CCSurface();

    virtual bool init();
    virtual void done();

    CCContext * getContext();
    void setContext(const ::std::shared_ptr<CCContext>& context);

    CCEvent<void (CCSurface *, int, int)> onSurfaceChanged;
    //EventQueue<PointerData, 10> pointerQueue;

    virtual void swapBuffer() = 0;

    int getWidth();
    int getHeight();
protected:
    CCSurface();

    ::std::shared_ptr<CCContext> _context;
    int _width;
    int _height;
};

#endif /* __CCSURFACE_H__ */
