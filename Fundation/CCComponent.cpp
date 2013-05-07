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

template < typename FunctionType >
CCComponent& CCComponent::registerFunction(const CCString& funcName, FunctionType func)
{
    auto functor = make_shared<CCSelectorVoidVoid<FunctionType>>();
    functor->pfn = func;
    _functions.insert(make_pair(funcName, functor));
    return *this;
}

template < typename FunctionType, typename Para1 >
CCComponent& CCComponent::registerFunction(const CCString& funcName, FunctionType func, Para1 para1)
{

    return *this;
}

template <typename ReturnType>
ReturnType CCComponent::callFunction(const CCString& funcName)
{
    ReturnType ret;
    return ret;
}

void CCComponent::callVoidFunction(const CCString& funcName)
{
    auto it = _functions.find(funcName);
    if (it != _functions.end())
    {
        CCSelectorVoidVoid<function<void()>>* pfn = (CCSelectorVoidVoid<function<void()>>*)it->second.get();
        (pfn->pfn)();
    }
    else
    {
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

    virtual CCString getName()
    {
        return "TestComponent";
    }
    virtual bool init()
    {
        function<void()> func = bind(&TestComponent::void_selector, this);
        registerFunction("void_selector", func);
        return true;
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

    com->callVoidFunction("void_selector");
}
