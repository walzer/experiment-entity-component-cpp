#ifndef __CC_SURFACE_H__
#define __CC_SURFACE_H__

#include "Event.h"

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
    struct PointerInterrupter
    {
        bool operator()(bool ret) { return ret; }
    };
public:
    virtual ~CCSurface();

    virtual bool init();
    virtual void done();

    CCContext * getContext();
    void setContext(const ::std::shared_ptr<CCContext>& context);

    Event<void (CCSurface *, int, int)> onSurfaceChanged;
    Event<bool (const PointerArgs &pointer), int, _UseLastValue, PointerInterrupter> onPointerDownEvent;

    //EventQueue<PointerData, 10> pointerQueue;

    virtual void swapBuffer() = 0;

    int getWidth();
    void setWidth(int w)
    {
        _width = w;
    }
    int getHeight();
    void setHeight(int h)
    {
        _height = h;
    }
protected:
    CCSurface();

    ::std::shared_ptr<CCContext> _context;
    int _width;
    int _height;
};

#endif /* __CC_SURFACE_H__ */
