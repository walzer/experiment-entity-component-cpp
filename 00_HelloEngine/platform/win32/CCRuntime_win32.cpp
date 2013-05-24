#include "pch.h"

#include "CCRuntime_win32.h"
#include "GameView.h"

using namespace std;

CCRuntime_win32::CCRuntime_win32()
    : CCRuntime()
{
}

bool CCRuntime_win32::init(void * appInstance)
{
    bool ret = BaseType::init(appInstance);
    if (ret)
    {
        auto surface = make_shared<GameView>();
        surface->init();
        this->addSurface(surface);
    }
    return ret;
}
