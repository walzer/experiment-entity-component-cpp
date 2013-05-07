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

template < typename FunctionTypeT >
CCComponent* CCComponent::registerFunction(const CCString& funcName, FunctionTypeT func)
{
    auto fonctionType = make_shared<FunctionType<FunctionTypeT>>();
    fonctionType->functor = func;
    _functions.insert(make_pair(funcName, fonctionType));
    return this;
}

CCComponent* CCComponent::unregisterFunction(const CCString& funcName)
{
    _functions.erase(funcName);
    return this;
}

template <typename ReturnType>
ReturnType CCComponent::callFunction(const CCString& funcName)
{
    FunctionType<function<ReturnType()>>* pfn = nullptr;
    auto it = _functions.find(funcName);
    if (it != _functions.end())
    {
        pfn = (FunctionType<function<ReturnType()>>*)it->second.get();
    }
    if (pfn)
    {
        return (pfn->functor)();
    }
    else
    {
        return ReturnType();
    }
}

template <typename ReturnType, typename Arg1>
ReturnType CCComponent::callFunction(const CCString& funcName, Arg1 aArg1)
{
    FunctionType<function<ReturnType(Arg1)>>* pfn = nullptr;
    auto it = _functions.find(funcName);
    if (it != _functions.end())
    {
        pfn = (FunctionType<function<ReturnType(Arg1)>>*)it->second.get();
    }
    if (pfn)
    {
        return (pfn->functor)(aArg1);
    }
    else
    {
        return ReturnType();
    }
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
    virtual bool init()
    {
        {
            function<void()> func = bind(&TestComponent::void_selector, this);
            registerFunction("void_selector", func);
        }
        {
            function<int()> func = bind(&TestComponent::int_selector, this);
            registerFunction("int_selector", func);
        }
        {
            function<void(int)> func = bind(&TestComponent::void_selector_int, this, placeholders::_1);
            registerFunction("void_selector_int", func);
        }
        {
            function<int(int)> func = bind(&TestComponent::int_selector_int, this, placeholders::_1);
            registerFunction("int_selector_int", func);
        }
        return true;
    }
    virtual void done()
    {
        unregisterFunction("void_selector");
        unregisterFunction("int_selector");
        unregisterFunction("void_selector_int");
        unregisterFunction("int_selector_int");
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
        _i += i;
        return _i;
    }
};
#undef  IMPLEMENT_CLASS
#define IMPLEMENT_CLASS     TestComponent
CCCOMPONENT_REGISTER_CREATOR;

void CCComponentTest()
{
    CCComponent::Ptr com = CCComponent::create("TestComponent");
    printf("%s\n",com->getName().c_str());
    com->init();
    auto b = bind(&TestComponent::void_selector, (TestComponent*)com.get());
    function<void ()> f = b;
    printf("%s\n", typeid(b).name());
    printf("%s\n", typeid(f).name());

    com->callFunction<void>("void_selector");
    com->callFunction<void, int>("void_selector_int", 1);
    int i = com->callFunction<int>("int_selector");
    printf("int_selector return %d\n", i);
    i = com->callFunction<int, int>("int_selector_int", 2);
    printf("int_selector_int(1) return %d\n", i);
    com->done();
}
