#include "pch.h"

#include "ComponentFactory.h"

#include "FooComponent.h"

namespace {;

using namespace cc;

CC_REGISTER_COMPONENT(FooComponent);

TEST(ComponentFactoryTest, componentCreation) {
    auto type = CC_CSTR("FooComponent");
    auto com = ComponentFactory::getInstance()->create(type);
    ASSERT_NE(nullptr, com);
    EXPECT_STREQ("FooComponent", com->getType().name);

    com.reset();
    ComponentFactory::getInstance()->unRegisterComponent(type);
    com = ComponentFactory::getInstance()->create(type);
    EXPECT_EQ(nullptr, com);
}

}   // namespace
