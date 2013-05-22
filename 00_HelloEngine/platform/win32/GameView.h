#pragma once

#include "windows.h"

#include "CCFoundation.h"

class GameView : public CCSurface
{
public:
    typedef CCSurface BaseType;

    GameView(void);
    virtual ~GameView(void);

    virtual bool init();
    virtual void done();

    void show();

    HWND getHandle() const { return _hwnd; }
private:
    HWND _hwnd;
};

