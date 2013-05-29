#ifndef __SCENE_UPDATE_COMPONENT_H__
#define __SCENE_UPDATE_COMPONENT_H__

#include "CCFoundation.h"

#include "CCUpdateComponent.h"

class SceneUpdateComponent : public CCUpdateComponent
{
protected:
    void update(float dt);
};

#endif  // __SCENE_UPDATE_COMPONENT_H__
