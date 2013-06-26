#ifndef __FOUNDATION_CC__TO_STRINGCOMPONENT_H__
#define __FOUNDATION_CC__TO_STRINGCOMPONENT_H__

#include <functional>
#include <map>
#include <memory>

#include "FundationMacrosH.h"
#include "String.h"

class CCContext;
class CCEntity;

class CCComponent : public ::std::enable_shared_from_this<CCComponent>
{
public:
    typedef CCComponent     ThisType;
    typedef ::std::shared_ptr<CCComponent> Ptr;

    CCComponent();
    virtual ~CCComponent();

    virtual bool init(CCContext* context);
    virtual void done();
    virtual String getName();
    
    const ::std::shared_ptr<CCEntity>& getOwner() const
    {
        return _owner;
    }
    void setOwner(const ::std::shared_ptr<CCEntity>& owner)
    {
        _owner = owner;
    }
    // Register a std::function object with funcName.
    template <
        typename Signature
    >
    void registerFunction(const String& funcName,
        const ::std::function<Signature>& func);

    // Unregister function object with name funcName.
    void unregisterFunction(const String& funcName);

    template <typename DerivedType>
    typename ::std::remove_reference<typename ::std::remove_pointer<DerivedType>::type>::type *
        as()
    {
        return static_cast<typename ::std::remove_reference <typename ::std::remove_pointer<DerivedType>::type>::type *>(this);
    }
    template <typename DerivedType>
    ::std::shared_ptr<typename ::std::remove_reference<typename ::std::remove_pointer<DerivedType>::type>::type>
        asShared()
    {
        return ::std::static_pointer_cast<typename ::std::remove_reference<typename ::std::remove_pointer<DerivedType>::type>::type>(this->shared_from_this());
    }

    template <
        typename Signature
    >
    ::std::function<Signature> findFunction(const String& funcName);

    // Call a registered function named funcName with none arguments.
    template <
        typename ReturnType
    >
    ReturnType callFunction(const String& funcName);

    // Call a registered function named funcName with 1-4 arguments.
    // VC11(VC2012) limits bind arguments to 5.
#define CCCOMPONENT_DECLARE_CALL_FUNCTION(ReturnType, ...) \
    template <typename ReturnType, CC_TYPES_WITH_TYPENAME(__VA_ARGS__)> \
    ReturnType callFunction(const String& funcName, __VA_ARGS__)
    CCCOMPONENT_DECLARE_CALL_FUNCTION(ReturnType, Arg1);
    CCCOMPONENT_DECLARE_CALL_FUNCTION(ReturnType, Arg1, Arg2);
    CCCOMPONENT_DECLARE_CALL_FUNCTION(ReturnType, Arg1, Arg2, Arg3);
    CCCOMPONENT_DECLARE_CALL_FUNCTION(ReturnType, Arg1, Arg2, Arg3, Arg4);
#undef CCCOMPONENT_DECLARE_CALL_FUNCTION

public:
    // static functions
    typedef Ptr (CreatorSignature)();
    typedef ::std::function<CreatorSignature>   Creator;
    typedef ::std::map<String, Creator>   CreatorEntry;

    static Ptr create(const String& typeName);
    static CreatorEntry& getCreatorEntry();
    static void registerCreator(const String& typeName, const Creator& creator);

public:
    // Register helper class template.
    template <
        typename ComponentType
    >
    struct Register
    {
        Register(const String& typeName);
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
    ::std::map<String, ::std::shared_ptr<FunctionTypeBase>> _functions;
    ::std::shared_ptr<CCEntity> _owner;
};

void CCComponentTest();

// Define CCCOMPONENT_REGISTER_CREATOR macro.
#define CC_TO_STRINGCOMPONENT_REGISTER_CREATOR_IMPL(typeName) \
    static CCComponent::Register<typeName> CC_GLUE(s_registerCreator, typeName)(CC_TO_STRING(typeName))
#define CCCOMPONENT_REGISTER_CREATOR \
    CC_TO_STRINGCOMPONENT_REGISTER_CREATOR_IMPL(IMPL_CLASS)
// Define CCCOMPONENT_REGISTER_CREATOR macro.

// Define CCCOMPONENT_REGISTER_MEMBER_FUNCTION macros.
#define CC_TO_STRINGCOMPONENT_REGISTER_MEMBER_FUNCTION_1(funcName, returnType) \
    registerFunction<returnType()>(CC_TO_STRING(funcName), \
        ::std::bind(& IMPL_CLASS :: funcName, this))
#define CC_TO_STRINGCOMPONENT_REGISTER_MEMBER_FUNCTION_ARGS(funcName, returnType, ...) \
    registerFunction<returnType(__VA_ARGS__)>(CC_TO_STRING(funcName), \
        ::std::bind(&IMPL_CLASS::funcName, this, CC_TYPES_TO_PLACEHOLDER(__VA_ARGS__)))
#define CC_TO_STRINGCOMPONENT_REGISTER_MEMBER_FUNCTION_2(funcName, returnType, ...) \
    CC_TO_STRINGCOMPONENT_REGISTER_MEMBER_FUNCTION_ARGS(funcName, returnType, __VA_ARGS__)
#define CC_TO_STRINGCOMPONENT_REGISTER_MEMBER_FUNCTION_3(funcName, returnType, ...) \
    CC_TO_STRINGCOMPONENT_REGISTER_MEMBER_FUNCTION_ARGS(funcName, returnType, __VA_ARGS__)
#define CC_TO_STRINGCOMPONENT_REGISTER_MEMBER_FUNCTION_4(funcName, returnType, ...) \
    CC_TO_STRINGCOMPONENT_REGISTER_MEMBER_FUNCTION_ARGS(funcName, returnType, __VA_ARGS__)
#define CC_TO_STRINGCOMPONENT_REGISTER_MEMBER_FUNCTION_5(funcName, returnType, ...) \
    CC_TO_STRINGCOMPONENT_REGISTER_MEMBER_FUNCTION_ARGS(funcName, returnType, __VA_ARGS__)
#define CCCOMPONENT_REGISTER_MEMBER_FUNCTION(funcName, ...) \
    CC_EXPEND(CC_APPEND_NARGS(CC_TO_STRINGCOMPONENT_REGISTER_MEMBER_FUNCTION_, __VA_ARGS__)(funcName, __VA_ARGS__ ))
// Define CCCOMPONENT_REGISTER_MEMBER_FUNCTION macros.

// #include "CCComponent.inl"

template <
    typename Signature
>
void CCComponent::registerFunction(const String& funcName,
    const ::std::function<Signature>& func)
{
    auto fonctionType = ::std::make_shared<FunctionType<::std::function<Signature>>>();
    fonctionType->functor = func;
    _functions.insert(::std::make_pair(funcName, fonctionType));
}

template <
    typename Signature
>
::std::function<Signature>
CCComponent::findFunction(const String& funcName)
{
    FunctionType<::std::function<Signature>>* pfn = nullptr;
    auto it = _functions.find(funcName);
    if (it != _functions.end())
    {
        pfn = (FunctionType<::std::function<Signature>>*)it->second.get();
    }
    if (pfn)
    {
        return pfn->functor;
    }
    else
    {
        return ::std::function<Signature>();
    }
}

template <typename ReturnType>
ReturnType CCComponent::callFunction(const String& funcName)
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

#define CCCOMPONENT_IMPL_CALL_FUNCTION(ReturnType, ...) \
template <typename ReturnType, CC_TYPES_WITH_TYPENAME(__VA_ARGS__)> \
ReturnType CCComponent::callFunction(const String& funcName, CC_TYPES_APPEND_PARA(__VA_ARGS__)) \
{ \
    FunctionType<::std::function<ReturnType(__VA_ARGS__)>>* pfn = nullptr; \
    auto it = _functions.find(funcName); \
    if (it != _functions.end()) \
    { \
        pfn = (FunctionType<::std::function<ReturnType(__VA_ARGS__)>>*)it->second.get(); \
    } \
    if (pfn) \
    { \
        return (pfn->functor)(CC_TYPES_TO_PARA(__VA_ARGS__)); \
    } \
    else \
    { \
        return ReturnType(); \
    } \
}

CCCOMPONENT_IMPL_CALL_FUNCTION(ReturnType, Arg1);
CCCOMPONENT_IMPL_CALL_FUNCTION(ReturnType, Arg1, Arg2);
CCCOMPONENT_IMPL_CALL_FUNCTION(ReturnType, Arg1, Arg2, Arg3);
CCCOMPONENT_IMPL_CALL_FUNCTION(ReturnType, Arg1, Arg2, Arg3, Arg4);
#undef CCCOMPONENT_IMPL_CALL_FUNCTION

template <
    typename ComponentType
>
CCComponent::Register<ComponentType>::Register(const String& typeName)
{
    auto creator = []()->CCComponent::Ptr
    {
        return ::std::make_shared<ComponentType>();
    };
    CCComponent::registerCreator(typeName, creator);
}


#endif  // __FOUNDATION_CC__TO_STRINGCOMPONENT_H__
