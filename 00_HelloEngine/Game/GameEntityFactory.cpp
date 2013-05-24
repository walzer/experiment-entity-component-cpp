#include "pch.h"

#include "GameEntityFactory.h"

using namespace std;

::std::shared_ptr<CCEntity> createScene(CCContext *context, const ::std::string &def)
{
    ::std::shared_ptr<CCEntity> ret;
    if (def == "GameScene")
    {
        ret = make_shared<CCEntity>();

        ret->addComponent(CCComponent::create("CCRenderComponent"));
        auto entityManager = static_cast<CCEntityManager*>(context->get("entity_manager"));
        entityManager->add(ret);
    }

    return ret;
}
