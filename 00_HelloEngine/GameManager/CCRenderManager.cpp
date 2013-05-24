#include "stdafx.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "CCRenderManager.h"

using namespace std;

CCRenderManager::~CCRenderManager()
{
}

bool CCRenderManager::init(CCContext * context)
{
    beginDelegate = context->preDrawEvent.add(bind(&CCRenderManager::begin, this), AT_FRONT);
    endDelegate = context->postDrawEvent.add(bind(&CCRenderManager::end, this));
    return true;
}

void CCRenderManager::done(CCContext * context)
{
    context->preDrawEvent.remove(beginDelegate);
    context->postDrawEvent.remove(endDelegate);
}

void CCRenderManager::begin()
{
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CCRenderManager::end()
{

}