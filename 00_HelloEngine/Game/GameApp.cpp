#include "stdafx.h"

#include "GameApp.h"

using namespace std;

bool GameApp::init()
{
    CCRuntime* runtime = CCRuntime::getInstance();
    _onSurfaceCreated = runtime->onSurfaceCreated.add(bind(&GameApp::onSurfaceCreated, this, placeholders::_1));
    return true;
}

void GameApp::done()
{
    CCRuntime* runtime = CCRuntime::getInstance();
    runtime->onSurfaceCreated.remove(_onSurfaceCreated);
}

void GameApp::onSurfaceCreated(CCSurface *surface)
{
    auto context = make_shared<CCContext>();
    context->add(make_shared<CCEntityManager>(), "entity_manager");
    surface->setContext(context);
}
