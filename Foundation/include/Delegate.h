#ifndef __FOUNDATION__CC_DELEGATE_H__
#define __FOUNDATION__CC_DELEGATE_H__

#include "CppStl.h"

#include "FundationMacrosH.h"
#include "Uncopyable.h"

namespace cc {;

enum CCDelegateAtPosition : char
{
    AT_BACK,
    AT_FRONT
};

class CCDelegateBase
{
    Uncopyable uncopyable;
public:
    typedef CCDelegateBase ThisType;
    typedef ::std::shared_ptr<CCDelegateBase> Ptr;

    inline CCDelegateBase();
    inline virtual ~CCDelegateBase();

    inline bool getEnabledStatus() const;
    inline void setEnabledStatus(bool enable);
    inline bool isGrouped() const;
    inline CCDelegateAtPosition atPosition() const;

protected:
    bool _enabled;
    bool _grouped;
    CCDelegateAtPosition _atPosition;
};

template <
    typename FunctionType,
    typename GroupType,
    typename IteratorType
>
class CCDelegate : public CCDelegateBase
{
public:
    FunctionType        function;
    GroupType           group;
    IteratorType        iterator;

    CCDelegate(const FunctionType& func,
        bool grouped,
        CCDelegateAtPosition atPosition);
};

class _UseLastValue
{
public:
    template <typename T>
    T operator() (const T& , const T& last)
    {
        return last;
    }
};

class _InvokeNoneInterrupt
{
public:
    template <typename T>
    bool operator() (const T& result)
    {
        return false;
    }
};

template <
    typename DelegateFunction,
    typename ResultType, 
    typename ResultCombiner = _UseLastValue,
    typename InvokeInterrupter = _InvokeNoneInterrupt
>
class CCDelegateInvoke
{
public:
    CCDelegateInvoke();

    bool invoke(const DelegateFunction& func);
    
#define CCDELEGATE_INVOKE_DEFINE_INVOKE_ARGS(...) \
    template < CC_TYPES_WITH_TYPENAME(__VA_ARGS__) > \
    bool invoke(const DelegateFunction& func, CC_TYPES_APPEND_PARAS(__VA_ARGS__)) \
    { \
        ResultType value = func(CC_TYPES_TO_PARAS(__VA_ARGS__)); \
        _result = _combiner(_result, value); \
        return _interrupter(value); \
    }
    CCDELEGATE_INVOKE_DEFINE_INVOKE_ARGS(Arg1);
    CCDELEGATE_INVOKE_DEFINE_INVOKE_ARGS(Arg1, Arg2);
    CCDELEGATE_INVOKE_DEFINE_INVOKE_ARGS(Arg1, Arg2, Arg3);
    CCDELEGATE_INVOKE_DEFINE_INVOKE_ARGS(Arg1, Arg2, Arg3, Arg4);
#undef CCDELEGATE_INVOKE_DEFINE_INVOKE_ARGS

    ResultType& getResult();

private:
    ResultType _result;
    ResultCombiner _combiner;
    InvokeInterrupter _interrupter;
};

}   // namespace cc

// #include "CCDelegate.inl"

namespace cc {;

////////////////////////////////////////////////////////////////////////////////
// Implement CCDelegateBase
CCDelegateBase::CCDelegateBase()
    : _enabled(true)
{
}

CCDelegateBase::~CCDelegateBase()
{
}

bool CCDelegateBase::getEnabledStatus() const
{
    return _enabled;
}

void CCDelegateBase::setEnabledStatus(bool enable)
{
    _enabled = enable;
}

bool CCDelegateBase::isGrouped() const
{
    return _grouped;
}

CCDelegateAtPosition CCDelegateBase::atPosition() const
{
    return _atPosition;
}

////////////////////////////////////////////////////////////////////////////////
// Implement template <> CCDelegate
template <
    typename FunctionType,
    typename GroupType,
    typename IteratorType
>
CCDelegate<FunctionType, GroupType, IteratorType>::CCDelegate(
    const FunctionType& func,
    bool grouped,
    CCDelegateAtPosition atPosition)
    : function(func)
{
    _grouped    = grouped;
    _atPosition = atPosition;
}

////////////////////////////////////////////////////////////////////////////////
// Implement template <> CCDelegateInvoke
template <
    typename DelegateFunction,
    typename ResultType, 
    typename ResultCombiner,
    typename InvokeInterrupter
>
CCDelegateInvoke<DelegateFunction, ResultType, ResultCombiner, InvokeInterrupter>::CCDelegateInvoke()
    : _result()
{
}

template <
    typename DelegateFunction,
    typename ResultType, 
    typename ResultCombiner,
    typename InvokeInterrupter
>
bool CCDelegateInvoke<DelegateFunction, ResultType, ResultCombiner, InvokeInterrupter>::invoke(const DelegateFunction& func)
{
    ResultType value = func();
    _result = _combiner(_result, value);
    return _interrupter(value);
}

template <
    typename DelegateFunction,
    typename ResultType, 
    typename ResultCombiner,
    typename InvokeInterrupter
>
ResultType& CCDelegateInvoke<DelegateFunction, ResultType, ResultCombiner, InvokeInterrupter>::getResult()
{
    return _result;
}

////////////////////////////////////////////////////////////////////////////////
// partial specialize template <> CCDelegateInvoke for void return type
template <
    typename DelegateFunction,
    typename ResultCombiner,
    typename InvokeInterrupter
>
class CCDelegateInvoke <DelegateFunction, void, ResultCombiner, InvokeInterrupter>
{
public:

    bool invoke(const DelegateFunction& func)
    {
        func();
        return false;
    }

#define CCDELEGATE_INVOKE_DEFINE_INVOKE_ARGS(...) \
    template < CC_TYPES_WITH_TYPENAME(__VA_ARGS__) > \
    bool invoke(const DelegateFunction& func, CC_TYPES_APPEND_PARAS(__VA_ARGS__)) \
    { \
        func(CC_TYPES_TO_PARAS(__VA_ARGS__)); \
        return false; \
    }
    CCDELEGATE_INVOKE_DEFINE_INVOKE_ARGS(Arg1);
    CCDELEGATE_INVOKE_DEFINE_INVOKE_ARGS(Arg1, Arg2);
    CCDELEGATE_INVOKE_DEFINE_INVOKE_ARGS(Arg1, Arg2, Arg3);
    CCDELEGATE_INVOKE_DEFINE_INVOKE_ARGS(Arg1, Arg2, Arg3, Arg4);
#undef CCDELEGATE_INVOKE_DEFINE_INVOKE_ARGS

    void getResult()
    {
    }
};

}   // namespace cc

#endif  // __FOUNDATION__CC_DELEGATE_H__
