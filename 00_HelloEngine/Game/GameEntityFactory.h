#ifndef __GAME_ENTITY_FACTORY_H__
#define __GAME_ENTITY_FACTORY_H__

#include <memory>

#include "String.h"

class CCContext;
class CCEntity;

::std::shared_ptr<CCEntity> createSprite(CCContext *context, CCEntity *parent);

::std::shared_ptr<CCEntity> createScene(CCContext *context, const String &def);

::std::shared_ptr<CCEntity> createPlayer(CCContext *context, const String &def, const ::std::shared_ptr<CCEntity> &parent);

//::std::shared_ptr<CCEntity> createEnemy(CCContext *context, const String &def);

//::std::shared_ptr<CCEntity> createBullet(CCContext *context, const String &def);

#endif  // __GAME_ENTITY_FACTORY_H__
