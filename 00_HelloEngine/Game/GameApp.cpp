#include "pch.h"

#include "GameApp.h"

#include "GameManager/CCRenderManager.h"
#include "GameEntityFactory.h"

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
    context->add(make_shared<CCEntityManager>(), "CCEntityManager");
    context->add(make_shared<CCRenderManager>(), "CCRenderManager");
    surface->setContext(context);

    auto gameScene = createScene(context.get(), "GameScene");
    createPlayer(context.get(), "Player1", gameScene);
}
