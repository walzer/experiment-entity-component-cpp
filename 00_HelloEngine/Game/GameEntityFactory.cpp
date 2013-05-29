#include "pch.h"

#include "GameEntityFactory.h"

#include "GameComponent/CCRenderComponent.h"

using namespace std;

shared_ptr<CCEntity> createScene(CCContext *context, const CCString &def)
{
    shared_ptr<CCEntity> ret;
    if (def == "GameScene")
    {
        ret = make_shared<CCEntity>();

        ret->addComponent(CCComponent::create("CCTransformComponent"));

        auto entityManager = static_cast<CCEntityManager*>(context->get("CCEntityManager"));
        entityManager->add(ret);

        auto background = createSprite(context, ret.get());
        auto renderCom = background->findComponent<CCRenderComponent>("class CCRenderComponent");
        float verticesData[] = 
        {
            -1.0f, -1.0f,  0.0f,     0.1f, 0.3f, 1.0f,
             1.0f, -1.0f,  0.0f,     0.1f, 0.3f, 1.0f,
            -1.0f,  1.0f,  0.0f,     0.6f, 0.8f, 1.0f,
             1.0f, -1.0f,  0.0f,     0.1f, 0.3f, 1.0f,
            -1.0f,  1.0f,  0.0f,     0.6f, 0.8f, 1.0f,
             1.0f,  1.0f,  0.0f,     0.6f, 0.8f, 1.0f,
            //-0.4f,-0.4f,0.0f,
            //0.4f ,-0.4f,0.0f,
            //0.0f ,0.4f ,0.0f
        };
        auto vertices = make_shared<CCVertices>();
        vertices->init(verticesData, sizeof(verticesData) / sizeof(float), 6);
        vertices->
            resetOffset()->
            resetOffset(CCVertices::DataFlag::POSITION, 0)->
            resetOffset(CCVertices::DataFlag::TEXTURE, 3);
        renderCom->setVertices(vertices);
    }

    return ret;
}

shared_ptr<CCEntity> createSprite(CCContext *context, CCEntity *parent)
{
    shared_ptr<CCEntity> ret = make_shared<CCEntity>();

    ret->addComponent(CCComponent::create("CCTransformComponent"));
    ret->addComponent(CCComponent::create("CCRenderComponent"));

    auto entityManager = static_cast<CCEntityManager*>(context->get("CCEntityManager"));
    entityManager->add(ret);

    if (parent)
    {
        parent->addChild(ret);
    }

    return ret;
}
