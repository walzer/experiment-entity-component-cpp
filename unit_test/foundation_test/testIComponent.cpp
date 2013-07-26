#include "pch.h"

#include "IComponent.h"

using namespace cc;

class FooComponent: public IComponent {
public:
    FooComponent() {
        setTypeName(make_shared<String>("FooComponent"));
    }

protected:
    bool _onInit(const IDom *) {
        setName(make_shared<String>("Foo"));
        return true;
    }

    void _onEnable() {}
    void _onDisable() {}
    void _onDone() {}
};

class FooContainer: public IComponentContainer {
};

TEST(IComponentTest, testMemberFunction) {
    FooComponent foo;

    EXPECT_EQ(false, foo.inited());
    EXPECT_EQ(false, foo.enabled());
    EXPECT_EQ(nullptr, foo.getOwner());
    EXPECT_EQ(nullptr, foo.getName());
    EXPECT_STREQ("FooComponent", foo.getTypeName()->c_str());

    ASSERT_EQ(true, foo.init());
}
