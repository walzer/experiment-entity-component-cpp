#ifndef __GAME_APP_H__
#define __GAME_APP_H__

#include "CCFoundation.h"

class GameApp
{
public:
    bool init();
    void done();

private:
    void onSurfaceCreated(CCSurface *surface);
    CCDelegateHandler _onSurfaceCreated;
};

#endif  // __GAME_APP_H__
