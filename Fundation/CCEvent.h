#ifndef __FUNDATION__CCEVENT_H__
#define __FUNDATION__CCEVENT_H__

#include <algorithm>
#include <functional>
#include <list>
#include <map>
#include <memory>

#include <tuple>
#include <typeinfo>

#include "CCFundationMacrosH.h"
#include "CCString.h"

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

    CCDelegateBase()
        : _enabled(true)
    {
    }

    virtual ~CCDelegateBase()
    {
    }

    bool getEnabledStatus() const
    {
        return _enabled;
    }

    void setEnabledStatus(bool enable)
    {
        _enabled = enable;
    }

    bool isGrouped() const
    {
        return _grouped;
    }

    CCDelegateAtPosition atPosition() const
    {
        return _atPosition;
    }

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
        CCDelegateAtPosition atPosition)
        : function(func)
    {
        _grouped    = grouped;
        _atPosition = atPosition;
    }
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
    CCDelegateInvoke()
        : _result()
    {
    }

    bool invoke (const DelegateFunction& func)
    {
        ResultType value = func();
        _result = _combiner(_result, value);
        return _interrupter(value);
    }
    
    template <
        typename Arg1
    >
    bool invoke(const DelegateFunction& func, Arg1 arg1)
    {
        ResultType value = func(arg1);
        _result = _combiner(_result, value);
        return _interrupter(value);
    }

    ResultType& getResult()
    {
        return _result;
    }

private:
    ResultType _result;
    ResultCombiner _combiner;
    InvokeInterrupter _interrupter;
};

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

class CCDelegateHandler
{
public:
    typedef ::std::weak_ptr<CCDelegateBase> HandlerType;

    CCDelegateHandler()
    {
    }

    CCDelegateHandler(const HandlerType& handler)
    {
        _delegateBase = handler;
    }

    void disable()
    {
        CCDelegateBase::Ptr delegateBase = _delegateBase.lock();
        if (delegateBase)
        {
            delegateBase->setEnabledStatus(false);
        }
    }

    bool enable()
    {
        CCDelegateBase::Ptr delegateBase = _delegateBase.lock();
        if (delegateBase)
        {
            delegateBase->setEnabledStatus(true);
            return true;
        }
        return false;
    }

    bool enabled() const
    {
        if (_delegateBase.expired())
        {
            return false;
        }
        return _delegateBase.lock()->getEnabledStatus();
    }

    CCDelegateBase::Ptr getDelegateBase() const
    {
        return (_delegateBase.expired()) ? 
            CCDelegateBase::Ptr() : _delegateBase.lock();
    }

    bool operator==(const CCDelegateHandler& other) const
    {
        CCDelegateBase::Ptr l(_delegateBase.lock());
        CCDelegateBase::Ptr r(other._delegateBase.lock());
        return l == r;
    }

    bool operator!=(const CCDelegateHandler& other) const
    {
        return !(*this == other);
    }

    bool operator<(const CCDelegateHandler& other) const
    {
        CCDelegateBase::Ptr l(_delegateBase.lock());
        CCDelegateBase::Ptr r(other._delegateBase.lock());
        return l < r;
    }

    void swap(CCDelegateHandler &other)
    {
        ::std::swap(_delegateBase, other._delegateBase);
    }
private:
    HandlerType _delegateBase;
};

template <
    typename Signature,
    typename GroupType = int,
    typename ResultCombiner = _UseLastValue,
    typename InvokeInterrupter = _InvokeNoneInterrupt
>
class CCEvent1
{
    // Uncopyable
    CCEvent1(const CCEvent1 &);
    CCEvent1 &operator = (const CCEvent1 &);
public:
    typedef CCEvent1<Signature, GroupType> ThisType;
    typedef ::std::function<Signature> DelegateFunction;
    typedef typename DelegateFunction::result_type  ResultType;
    typedef ::std::list<::std::shared_ptr<CCDelegateBase>> DelegateList;
    typedef CCDelegate<DelegateFunction, GroupType, DelegateList::iterator> Delegate;
    
    CCEvent1()
        : _raising(false)
    {
    }

    CCDelegateHandler add(::std::shared_ptr<Delegate>& delegate)
    {
        DelegateList& list = getDelegateList(delegate.get());
        if (delegate->atPosition() == AT_BACK)
        {
            list.push_back(delegate);
            delegate->iterator = (++list.rbegin()).base();
        }
        else
        {
            list.push_front(delegate);
            delegate->iterator = list.begin();
        }
        return CCDelegateHandler(delegate);
    }

    CCDelegateHandler add(const DelegateFunction& func,
        CCDelegateAtPosition atPosition = AT_BACK)
    {
        auto delegate = ::std::make_shared<Delegate>(func, false,  atPosition);
        return add(delegate);
    }

    CCDelegateHandler add(const GroupType& group,
        const DelegateFunction& func,
        CCDelegateAtPosition atPosition = AT_BACK)
    {
        auto delegate = ::std::make_shared<Delegate>(func, true,  atPosition);
        delegate->group = group;
        return add(delegate);
    }

    ResultType operator () ()
    {
        CCDelegateInvoke<DelegateFunction, ResultType, ResultCombiner, InvokeInterrupter> invoker;
        _raising = true;
        auto end = _frontList.end();
        auto groupEnd = _groupedLists.end();
        Delegate * delegate = nullptr;
        for (_raisingIter = _frontList.begin(); _raisingIter != end; )
        {
            delegate = (Delegate*)(*_raisingIter++).get();
            if (delegate->getEnabledStatus())
            {
                if (invoker.invoke(delegate->function))
                {
                    goto EVENT_INTERRUPTED;
                }
            }
        }
        for (auto groupIt = _groupedLists.begin(); groupIt != groupEnd; ++groupIt)
        {
            DelegateList& list = groupIt->second;
            end = list.end();
            for (_raisingIter = list.begin(); _raisingIter != end; )
            {
                delegate = (Delegate*)(*_raisingIter++).get();
                if (delegate->getEnabledStatus())
                {
                    if (invoker.invoke(delegate->function))
                    {
                        goto EVENT_INTERRUPTED;
                    }
                }
            }
        }
        end = _backList.end();
        for (_raisingIter = _backList.begin(); _raisingIter != end; )
        {
            delegate = (Delegate*)(*_raisingIter++).get();
            if (delegate->getEnabledStatus())
            {
                if (invoker.invoke(delegate->function))
                {
                    goto EVENT_INTERRUPTED;
                }
            }
        }
        EVENT_INTERRUPTED:
        _raising = false;
        return invoker.getResult();
    }

    template<
        typename Arg1
    >
    ResultType operator () (Arg1 arg1)
    {
        CCDelegateInvoke<DelegateFunction, ResultType, ResultCombiner, InvokeInterrupter> invoker;
        _raising = true;
        bool interrupt = false;
        auto end = _frontList.end();
        auto groupEnd = _groupedLists.end();
        Delegate * delegate = nullptr;
        for (_raisingIter = _frontList.begin(); _raisingIter != end; )
        {
            delegate = (Delegate*)(*_raisingIter++).get();
            if (delegate->getEnabledStatus())
            {
                if (invoker.invoke(delegate->function, arg1))
                {
                    goto EVENT_INTERRUPTED;
                }
            }
        }
        for (auto groupIt = _groupedLists.begin(); groupIt != groupEnd; ++groupIt)
        {
            DelegateList& list = groupIt->second;
            end = list.end();
            for (_raisingIter = list.begin(); _raisingIter != end; )
            {
                delegate = (Delegate*)(*_raisingIter++).get();
                if (delegate->getEnabledStatus())
                {
                    if (invoker.invoke(delegate->function, arg1))
                    {
                        goto EVENT_INTERRUPTED;
                    }
                }
            }
        }
        end = _backList.end();
        for (_raisingIter = _backList.begin(); _raisingIter != end; )
        {
            delegate = (Delegate*)(*_raisingIter++).get();
            if (delegate->getEnabledStatus())
            {
                if (invoker.invoke(delegate->function, arg1))
                {
                    goto EVENT_INTERRUPTED;
                }
            }
        }
        EVENT_INTERRUPTED:
        _raising = false;
        return invoker.getResult();
    }

    void remove(const CCDelegateHandler& handler)
    {
        CCDelegateBase::Ptr delegateBase = handler.getDelegateBase();
        if (delegateBase)
        {
            Delegate* delegate = static_cast<Delegate*>(delegateBase.get());
            if (_raising && delegate->iterator == _raisingIter)
            {
                ++_raisingIter;
            }
            getDelegateList(delegate).erase(delegate->iterator);
        }
    }

protected:
    DelegateList& getDelegateList(Delegate* delegate)
    {
        return (delegate->isGrouped()) ? _groupedLists[delegate->group] : 
            (delegate->atPosition() == AT_BACK) ? _backList : _frontList;
    }

    DelegateList _backList;
    DelegateList _frontList;
    ::std::map<GroupType, DelegateList> _groupedLists;

    bool _raising;
    DelegateList::iterator _raisingIter;
};

template <
    typename EventSender,
    typename EventArg
>
class CCEvent
{
public:
    typedef void (DelegateSignature)(EventSender*, const EventArg&);
    typedef ::std::function<DelegateSignature> Delegate;
    typedef ::std::tuple<Delegate, CCString> DelegateInfo;

    typedef CCEvent<EventSender, EventArg> ThisType;

    ThisType& add(const Delegate& delegate)
    {
        return add(delegate, delegate.target_type().name());
    }

    ThisType& add(const Delegate& delegate, const CCString& name)
    {
        _delegates.emplace_back(delegate, name);
        return *this;
    }

    Delegate* get(const CCString& name)
    {
        Delegate* ret = nullptr;
        for(auto it = _delegates.begin(); it != _delegates.end(); ++it)
        {
            if (::std::get<1>(*it) != name) continue;
            ret = &::std::get<0>(*it);
        }
        return ret;
    }

    void raise(EventSender* sender, const EventArg& args)
    {
        ::std::for_each(_delegates.begin(), _delegates.end(),[=, & args](const DelegateInfo& info)
        {
            ::std::get<0>(info)(sender, args);
        });
    }

    ThisType& remove(const CCString& name)
    {
        for(auto it = _delegates.begin(); it != _delegates.end(); ++it)
        {
            if (::std::get<1>(*it) != name) continue;
            _delegates.erase(it);
            break;
        };
        return *this;
    }

protected:
    ::std::list<DelegateInfo> _delegates;
};


void CCEventTest();

// #include "CCEvent.inl"

#endif /* __FUNDATION__CCEVENT_H__ */
