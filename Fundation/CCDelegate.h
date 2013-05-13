#ifndef __FUNDATION__CCDELEGATE_H__
#define __FUNDATION__CCDELEGATE_H__

#include <memory>

#include "CCFundationMacrosH.h"

enum CCDelegateAtPosition : char
{
    AT_BACK,
    AT_FRONT
};

class CCDelegateBase
{
    // Uncopyable
    CCDelegateBase(const CCDelegateBase &);
    CCDelegateBase &operator = (const CCDelegateBase &);
public:
    typedef CCDelegateBase ThisType;
    typedef ::std::shared_ptr<CCDelegateBase> Ptr;

    CCDelegateBase();
    virtual ~CCDelegateBase();

    bool getEnabledStatus() const;
    void setEnabledStatus(bool enable);
    bool isGrouped() const;
    CCDelegateAtPosition atPosition() const;

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
    
    template <
        typename Arg1
    >
    bool invoke(const DelegateFunction& func, Arg1 arg1)
    {
        ResultType value = func(arg1);
        _result = _combiner(_result, value);
        return _interrupter(value);
    }

    ResultType& getResult();

private:
    ResultType _result;
    ResultCombiner _combiner;
    InvokeInterrupter _interrupter;
};

// #include "CCDelegate.inl"

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

    template <
        typename Arg1
    >
    bool invoke(const DelegateFunction& func, Arg1 arg1)
    {
        func(arg1);
        return false;
    }

    void getResult()
    {
    }
};

#endif  // __FUNDATION__CCDELEGATE_H__
