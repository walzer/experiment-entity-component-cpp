#pragma once

#include "stdafx.h"

class GameView
{
public:
    GameView(void);
    ~GameView(void);

    bool init();
    void done();

    HWND getHandle() const { return mHandle; }
private:
    HWND mHandle;
};

