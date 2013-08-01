#include "pch.h"

#include "FooComponent.h"

CC_IMPL_COMPONENT_TYPE_INFO(FooComponent, nullptr, "abc", nullptr);

FooComponent::FooComponent():
    calledOnInit(0),
    calledOnEnable(0) {}

bool FooComponent::_onInit(const ::cc::IDom *) {
    ++calledOnInit;
    _setName(CC_CSTR("Foo"));
    return true;
}

void FooComponent::_onEnable() {
    ++calledOnEnable;
}

void FooComponent::_onDisable() {
    --calledOnEnable;
}

void FooComponent::_onDone() {
    --calledOnInit;
}

////////////////////////////////////////////////////////////////////////////////

namespace {;

using namespace cc;

class FooContainer: public IComponentContainer {
};

TEST(IComponentTest, testMemberFunction) {
    FooComponent foo;

    // Uninitial state.
    EXPECT_EQ(false, foo.inited());
    EXPECT_EQ(false, foo.enabled());
    EXPECT_EQ(nullptr, foo.getOwner());
    EXPECT_EQ(nullptr, foo.getName());
    auto info = foo.getTypeInfo();
    EXPECT_STREQ("FooComponent", info.name);
    EXPECT_STREQ("abc", (info.dependences)[0]);
    EXPECT_EQ(nullptr, (info.dependences)[1]);

    // Initialized state.
    ASSERT_EQ(true, foo.init());
    EXPECT_EQ(true, foo.inited());
    EXPECT_EQ(false, foo.enabled());
    EXPECT_EQ(nullptr, foo.getOwner());
    EXPECT_STREQ("Foo", foo.getName()->c_str());

    // Enabled state.
    foo.enable();
    EXPECT_EQ(true, foo.enabled());

    // Disable.
    foo.disable();
    EXPECT_EQ(true, foo.inited());
    EXPECT_EQ(false, foo.enabled());

    // Done.
    foo.done();
    EXPECT_EQ(false, foo.inited());
}

// Test call method for change state.
// int values form 0~3
// 0: init
// 1: enable
// 2: disable
// 3: done
typedef tuple<int, int, int, int> CompTestArgs;
class CompStateTest: public testing::TestWithParam<CompTestArgs> {};

TEST_P(CompStateTest, changeState) {
    const CompTestArgs &param = GetParam();
    vector<int> actions;
    actions.push_back(get<0>(param));
    actions.push_back(get<1>(param));
    actions.push_back(get<2>(param));
    actions.push_back(get<3>(param));
    cout << "Param is:";
    for_each(actions.begin(), actions.end(), [](int &v) { cout << " " << v; });
    cout << endl;

    FooComponent foo;
    for (unsigned i = 0; i < actions.size(); ++i) {
        switch (actions[i]) {
        case 0:     // init
            ASSERT_EQ(true, foo.init());
            ASSERT_EQ(1, foo.calledOnInit);
            ASSERT_EQ(true, foo.init());
            ASSERT_EQ(1, foo.calledOnInit);
            break;
        case 1:     // enable
            if (foo.inited()) {
                foo.enable();
                ASSERT_EQ(1, foo.calledOnEnable);
                foo.enable();
                ASSERT_EQ(1, foo.calledOnEnable);
            }
            break;
        case 2:     // disable
            foo.disable();
            ASSERT_EQ(0, foo.calledOnEnable);
            foo.disable();
            ASSERT_EQ(0, foo.calledOnEnable);
            break;
        case 3:     // done
            foo.done();
            ASSERT_EQ(0, foo.calledOnInit);
            ASSERT_EQ(0, foo.calledOnEnable);
            foo.done();
            ASSERT_EQ(0, foo.calledOnInit);
            ASSERT_EQ(0, foo.calledOnEnable);
            break;
        default:
            ASSERT_EQ(true, false) << "Get unexpect action code!";
            break;
        }
    }
}

INSTANTIATE_TEST_CASE_P(
    StateCheck, CompStateTest, testing::Combine(
        testing::Range(0, 4),
        testing::Range(0, 4),
        testing::Range(0, 4),
        testing::Range(0, 4)
    )
);

}   // namespace
