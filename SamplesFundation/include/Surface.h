#ifndef __RENDER_H__
#define __RENDER_H__

#include "EventHandler.h"

class Context;

struct PointerArgs
{
    int id;
    int x;
    int y;
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

class Surface
{
public:
    virtual ~Surface();

    Context * getContext();
    void setContext(Context * context);

    EventHandler<Surface, PointerData> pointerEvent;
    EventQueue<PointerData, 10> pointerQueue;

    void onEvents();

    int getWidth();
    int getHeight();
protected:
    Surface();

    Context * mContext;
    int mWidth;
    int mHeight;
};

#endif /* __RENDER_H__ */
