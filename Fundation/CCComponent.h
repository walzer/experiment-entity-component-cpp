#ifndef __FUNDATION__CCCOMPONENT_H__
#define __FUNDATION__CCCOMPONENT_H__

#include <functional>
#include <map>
#include <memory>

#include "CCFundationMacrosH.h"
#include "CCString.h"

class CCContext;
class CCEntity;

class CCComponent : public ::std::enable_shared_from_this<CCComponent>
{
public:
    typedef CCComponent     ThisType;
    typedef ::std::shared_ptr<CCComponent> Ptr;

    CCComponent();
    virtual ~CCComponent();

    virtual bool init();
    virtual void done();
    virtual CCString getName();
    
    // Register a std::function object with funcName.
    template <
        typename Signature
    >
    void registerFunction(const CCString& funcName,
        const ::std::function<typename Signature>& func);

    // Unregister function object with name funcName.
    void unregisterFunction(const CCString& funcName);

    template <
        typename Signature
    >
    ::std::function<typename Signature> findFunction(const CCString& funcName);

    // Call a registered function named funcName with none arguments.
    template <
        typename ReturnType
    >
    ReturnType callFunction(const CCString& funcName);

    // Call a registered function named funcName with 1-4 arguments.
    // VC11(VC2012) limits bind arguments to 5.
#define CCCOMPONENT_DECLARE_CALL_FUNCTION(ReturnType, ...) \
    template <typename ReturnType, CCTYPES_WITH_TYPENAME(__VA_ARGS__)> \
    ReturnType callFunction(const CCString& funcName, __VA_ARGS__)
    CCCOMPONENT_DECLARE_CALL_FUNCTION(ReturnType, Arg1);
    CCCOMPONENT_DECLARE_CALL_FUNCTION(ReturnType, Arg1, Arg2);
    CCCOMPONENT_DECLARE_CALL_FUNCTION(ReturnType, Arg1, Arg2, Arg3);
    CCCOMPONENT_DECLARE_CALL_FUNCTION(ReturnType, Arg1, Arg2, Arg3, Arg4);
#undef CCCOMPONENT_DECLARE_CALL_FUNCTION

public:
    // static functions
    typedef Ptr (CreatorSignature)();
    typedef ::std::function<CreatorSignature>   Creator;
    typedef ::std::map<CCString, Creator>   CreatorEntry;

    static Ptr create(const CCString& typeName);
    static CreatorEntry& getCreatorEntry();
    static void registerCreator(const CCString& typeName, const Creator& creator);

public:
    // Register helper class template.
    template <
        typename ComponentType
    >
    struct Register
    {
        Register(const CCString& typeName);
    };

private:
    struct FunctionTypeBase {};
    template <
        typename FunctionTypeT
    >
    struct FunctionType : public FunctionTypeBase
    {
        FunctionTypeT functor;
    };
    ::std::map<CCString, ::std::shared_ptr<FunctionTypeBase>> _functions;
    ::std::shared_ptr<CCEntity> _owner;
};

void CCComponentTest();

// Define CCCOMPONENT_REGISTER_CREATOR macro.
#define _CCCOMPONENT_REGISTER_CREATOR_IMPL(typeName) \
    static CCComponent::Register<typeName> CCGLUE(s_registerCreator, typeName)(CCTO_STRING(typeName))
#define CCCOMPONENT_REGISTER_CREATOR \
    _CCCOMPONENT_REGISTER_CREATOR_IMPL(IMPLEMENT_CLASS)
// Define CCCOMPONENT_REGISTER_CREATOR macro.

// Define CCCOMPONENT_REGISTER_MEMBER_FUNCTION macros.
#define _CCCOMPONENT_REGISTER_MEMBER_FUNCTION_1(funcName, returnType) \
    registerFunction<returnType()>(CCTO_STRING(funcName), \
        ::std::bind(& IMPLEMENT_CLASS :: funcName, this))
#define _TYPE_TO_HOLDER(n, type) ::std::placeholders::_##n
#define _CCCOMPONENT_REGISTER_MEMBER_FUNCTION_2(funcName, returnType, ...) \
    registerFunction<returnType(__VA_ARGS__)>(CCTO_STRING(funcName), \
        ::std::bind(&IMPLEMENT_CLASS::funcName, this, CCFOR_EACH_NUM(_TYPE_TO_HOLDER, __VA_ARGS__)))
#define _CCCOMPONENT_REGISTER_MEMBER_FUNCTION_3(funcName, returnType, ...) \
    _CCCOMPONENT_REGISTER_MEMBER_FUNCTION_2(funcName, returnType, __VA_ARGS__)
#define _CCCOMPONENT_REGISTER_MEMBER_FUNCTION_4(funcName, returnType, ...) \
    _CCCOMPONENT_REGISTER_MEMBER_FUNCTION_2(funcName, returnType, __VA_ARGS__)
#define _CCCOMPONENT_REGISTER_MEMBER_FUNCTION_5(funcName, returnType, ...) \
    _CCCOMPONENT_REGISTER_MEMBER_FUNCTION_2(funcName, returnType, __VA_ARGS__)
#define CCCOMPONENT_REGISTER_MEMBER_FUNCTION(funcName, ...) \
    CCAPPEND_NARGS(_CCCOMPONENT_REGISTER_MEMBER_FUNCTION_, __VA_ARGS__) CCEXPEND((funcName, __VA_ARGS__ ))
// Define CCCOMPONENT_REGISTER_MEMBER_FUNCTION macros.

// #include "CCComponent.inl"

template <
    typename Signature
>
void CCComponent::registerFunction(const CCString& funcName,
    const ::std::function<typename Signature>& func)
{
    auto fonctionType = ::std::make_shared<FunctionType<::std::function<typename Signature>>>();
    fonctionType->functor = func;
    _functions.insert(::std::make_pair(funcName, fonctionType));
}

template <
    typename Signature
>
::std::function<typename Signature>
CCComponent::findFunction(const CCString& funcName)
{
    FunctionType<::std::function<typename Signature>>* pfn = nullptr;
    auto it = _functions.find(funcName);
    if (it != _functions.end())
    {
        pfn = (FunctionType<::std::function<typename Signature>>*)it->second.get();
    }
    if (pfn)
    {
        return pfn->functor;
    }
    else
    {
        return ::std::function<typename Signature>();
    }
}

template <typename ReturnType>
ReturnType CCComponent::callFunction(const CCString& funcName)
{
    FunctionType<::std::function<ReturnType()>>* pfn = nullptr;
    auto it = _functions.find(funcName);
    if (it != _functions.end())
    {
        pfn = (FunctionType<::std::function<ReturnType()>>*)it->second.get();
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

#define CCCOMPONENT_IMPLEMENT_CALL_FUNCTION(ReturnType, ...) \
template <typename ReturnType, CCTYPES_WITH_TYPENAME(__VA_ARGS__)> \
ReturnType CCComponent::callFunction(const CCString& funcName, CCTYPES_APPEND_PARAS(__VA_ARGS__)) \
{ \
    FunctionType<::std::function<ReturnType(__VA_ARGS__)>>* pfn = nullptr; \
    auto it = _functions.find(funcName); \
    if (it != _functions.end()) \
    { \
        pfn = (FunctionType<::std::function<ReturnType(__VA_ARGS__)>>*)it->second.get(); \
    } \
    if (pfn) \
    { \
        return (pfn->functor)(CCTYPES_TO_PARAS(__VA_ARGS__)); \
    } \
    else \
    { \
        return ReturnType(); \
    } \
}

CCCOMPONENT_IMPLEMENT_CALL_FUNCTION(ReturnType, Arg1);
CCCOMPONENT_IMPLEMENT_CALL_FUNCTION(ReturnType, Arg1, Arg2);
CCCOMPONENT_IMPLEMENT_CALL_FUNCTION(ReturnType, Arg1, Arg2, Arg3);
CCCOMPONENT_IMPLEMENT_CALL_FUNCTION(ReturnType, Arg1, Arg2, Arg3, Arg4);
#undef CCCOMPONENT_IMPLEMENT_CALL_FUNCTION

template <
    typename ComponentType
>
CCComponent::Register<ComponentType>::Register(const CCString& typeName)
{
    auto creator = []()->CCComponent::Ptr
    {
        return ::std::make_shared<typename ComponentType>();
    };
    CCComponent::registerCreator(typeName, creator);
}


#endif  // __FUNDATION__CCCOMPONENT_H__
