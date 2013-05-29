#ifndef __GAME_ENTITY_FACTORY_H__
#define __GAME_ENTITY_FACTORY_H__

#include <memory>

#include "CCString.h"

class CCContext;
class CCEntity;

::std::shared_ptr<CCEntity> createScene(CCContext *context, const CCString &def);

::std::shared_ptr<CCEntity> createSprite(CCContext *context, CCEntity *parent);

::std::shared_ptr<CCEntity> createPlayer(CCContext *context, const CCString &def);

::std::shared_ptr<CCEntity> createEnemy(CCContext *context, const CCString &def);

::std::shared_ptr<CCEntity> createBullet(CCContext *context, const CCString &def);

#endif  // __GAME_ENTITY_FACTORY_H__
