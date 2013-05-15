#include "pch.h"

#include "CCComponent.h"

#include <typeinfo>

using namespace std;

#define IMPLEMENT_CLASS     CCComponent

CCCOMPONENT_REGISTER_CREATOR;

CCComponent::CCComponent()
    : _owner(nullptr)
{
}

CCComponent::~CCComponent()
{
}

CCString CCComponent::getName()
{
    return typeid(*this).name();
}

CCComponent* CCComponent::unregisterFunction(const CCString& funcName)
{
    _functions.erase(funcName);
    return this;
}

bool CCComponent::init()
{
    return true;
}

void CCComponent::done()
{
}

CCComponent::Ptr CCComponent::create(const CCString& typeName)
{
    CCComponent::Ptr ret;
    auto it = getCreatorEntry().find(typeName);
    if (it != getCreatorEntry().end())
    {
        ret = it->second();
    }
    return ret;
}

CCComponent::CreatorEntry& CCComponent::getCreatorEntry()
{
    static CCComponent::CreatorEntry s_entry;
    return s_entry;
}

void CCComponent::registerCreator(const CCString& comName, const Creator& creator)
{
    getCreatorEntry().insert(make_pair(comName, creator));
}

#undef  IMPLEMENT_CLASS
////////////////////////////////////////////////////////////////////////////////
class TestComponent : public CCComponent
{
    int _i;
public:
    typedef shared_ptr<TestComponent> Ptr;

    TestComponent()
        : _i(0)
    {
    }

    virtual CCString getName()
    {
        return "TestComponent";
    }
    virtual bool init();
    virtual void done()
    {
        unregisterFunction("void_selector");
        unregisterFunction("int_selector");
        unregisterFunction("void_selector_int");
        unregisterFunction("int_selector_int");
        unregisterFunction("int_selector_int2");
        unregisterFunction("int_selector_int3");
        unregisterFunction("int_selector_int4");
    }
    void void_selector()
    {
        printf("%s\n", __FUNCTION__);
    }
    int int_selector()
    {
        printf("%s\n", __FUNCTION__);
        return _i;
    }
    void void_selector_int(int i)
    {
        printf("%s\n", __FUNCTION__);
        _i = i;
    }
    int int_selector_int(int i)
    {
        printf("%s\n", __FUNCTION__);
        return _i + i;
    }
    int int_selector_int2(int i1, int& i2)
    {
        printf("%s\n", __FUNCTION__);
        return _i + i1 + i2;
    }
    int int_selector_int3(int i1, int i2, int i3)
    {
        printf("%s\n", __FUNCTION__);
        return _i + i1 + i2 + i3;
    }
    int int_selector_int4(int i1, int i2, int i3, int i4)
    {
        printf("%s\n", __FUNCTION__);
        return _i + i1 + i2 + i3 + i4;
    }
};
#define IMPLEMENT_CLASS     TestComponent
CCCOMPONENT_REGISTER_CREATOR;
bool TestComponent::init()
{
    registerFunction<void()>("void_selector", bind(&TestComponent::void_selector, this));
    CCCOMPONENT_REGISTER_MEMBER_FUNCTION(int_selector, int);
    CCCOMPONENT_REGISTER_MEMBER_FUNCTION(void_selector_int, void, int);
    registerFunction<int(int)>("int_selector_int", ::std::bind(&TestComponent::int_selector_int, this, ::std::placeholders::_1));
    CCCOMPONENT_REGISTER_MEMBER_FUNCTION(int_selector_int2, int, int, int);
    CCCOMPONENT_REGISTER_MEMBER_FUNCTION(int_selector_int3, int, int, int, int);
    CCCOMPONENT_REGISTER_MEMBER_FUNCTION(int_selector_int4, int, int, int, int, int);
    return true;
}

void CCComponentTest()
{
    CCComponent::Ptr com = CCComponent::create("TestComponent");
    printf("%s\n",com->getName().c_str());
    com->init();
    auto b = bind(&TestComponent::void_selector, (TestComponent*)com.get());
    function<void ()> f = b;
    printf("%s\n", typeid(b).name());
    printf("%s\n", typeid(f).name());

    auto findf = com->findFunction<void()>("void_selector");
    if (findf != nullptr)
    {
        findf();
    }
    auto findf2 = com->findFunction<void()>("void_selector2");
    if (findf2 != nullptr)
    {
        // findf2 will not be called
        findf2();
    }
    com->callFunction<void>("void_selector");
    com->callFunction<void, int>("void_selector_int", 1);
    int i = com->callFunction<int>("int_selector");
    printf("int_selector return %d\n", i);
    i = com->callFunction<int, int>("int_selector_int", 1);
    printf("int_selector_int(1) return %d\n", i);
    i = com->callFunction<int, int, int>("int_selector_int2", 1, 1);
    printf("int_selector_int2(1,1) return %d\n", i);
    i = com->callFunction<int, int, int, int>("int_selector_int3", 1, 1, 1);
    printf("int_selector_int3(1,1,1) return %d\n", i);
    i = com->callFunction<int, int, int, int, int>("int_selector_int4", 1, 1, 1, 1);
    printf("int_selector_int4(1,1,1,1) return %d\n", i);
    com->done();
}
