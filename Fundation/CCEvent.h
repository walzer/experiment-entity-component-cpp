#ifndef __FUNDATION__CCEVENT_H__
#define __FUNDATION__CCEVENT_H__

#include <functional>
#include <list>
#include <map>

#include "CCDelegate.h"
#include "CCString.h"

class CCDelegateHandler
{
public:
    typedef ::std::weak_ptr<CCDelegateBase> HandlerType;

    inline CCDelegateHandler();
    inline CCDelegateHandler(const HandlerType& handler);

    inline void disable();
    inline bool enable();
    inline bool enabled() const;
    inline CCDelegateBase::Ptr getDelegateBase() const;

    inline bool operator==(const CCDelegateHandler& other) const;
    inline bool operator!=(const CCDelegateHandler& other) const;
    inline bool operator<(const CCDelegateHandler& other) const;
    inline void swap(CCDelegateHandler &other);

private:
    HandlerType _delegateBase;
};

template <
    typename Signature,
    typename GroupType = int,
    typename Combiner = _UseLastValue,
    typename Interrupter = _InvokeNoneInterrupt
>
class CCEvent
{
    // Uncopyable
    CCEvent(const CCEvent &);
    CCEvent &operator = (const CCEvent &);
public:
    typedef CCEvent<Signature, GroupType> ThisType;
    typedef ::std::function<Signature> DelegateFunction;
    typedef typename DelegateFunction::result_type  ResultType;
    typedef ::std::list<::std::shared_ptr<CCDelegateBase>> DelegateList;
    typedef CCDelegate<DelegateFunction, GroupType, DelegateList::iterator> Delegate;
    
    CCEvent();

    CCDelegateHandler add(::std::shared_ptr<Delegate>& delegate);
    CCDelegateHandler add(const DelegateFunction& func,
        CCDelegateAtPosition atPosition = AT_BACK);
    CCDelegateHandler add(const GroupType& group,
        const DelegateFunction& func,
        CCDelegateAtPosition atPosition = AT_BACK);

    void remove(const CCDelegateHandler& handler);

    ResultType operator () ();

#define CCEVENT_DEFINE_OPERATOR_ARGS(...) \
    template< CCTYPES_WITH_TYPENAME(__VA_ARGS__) > \
    ResultType operator () (CCTYPES_APPEND_PARAS(__VA_ARGS__)) \
    { \
        CCDelegateInvoke<DelegateFunction, ResultType, Combiner, Interrupter> invoker; \
        _raising = true; \
        bool interrupt = false; \
        auto end = _frontList.end(); \
        auto groupEnd = _groupedLists.end(); \
        Delegate * delegate = nullptr; \
        for (_raisingIter = _frontList.begin(); _raisingIter != end; ) \
        { \
            delegate = (Delegate*)(*_raisingIter++).get(); \
            if (delegate->getEnabledStatus()) \
            { \
                if (invoker.invoke(delegate->function, CCTYPES_TO_PARAS(__VA_ARGS__))) \
                { \
                    goto EVENT_INTERRUPTED; \
                } \
            } \
        } \
        for (auto groupIt = _groupedLists.begin(); groupIt != groupEnd; ++groupIt) \
        { \
            DelegateList& list = groupIt->second; \
            end = list.end(); \
            for (_raisingIter = list.begin(); _raisingIter != end; ) \
            { \
                delegate = (Delegate*)(*_raisingIter++).get(); \
                if (delegate->getEnabledStatus()) \
                { \
                    if (invoker.invoke(delegate->function, CCTYPES_TO_PARAS(__VA_ARGS__))) \
                    { \
                        goto EVENT_INTERRUPTED; \
                    } \
                } \
            } \
        } \
        end = _backList.end(); \
        for (_raisingIter = _backList.begin(); _raisingIter != end; ) \
        { \
            delegate = (Delegate*)(*_raisingIter++).get(); \
            if (delegate->getEnabledStatus()) \
            { \
                if (invoker.invoke(delegate->function, CCTYPES_TO_PARAS(__VA_ARGS__))) \
                { \
                    goto EVENT_INTERRUPTED; \
                } \
            } \
        } \
        EVENT_INTERRUPTED: \
        _raising = false; \
        return invoker.getResult(); \
    }
    CCEVENT_DEFINE_OPERATOR_ARGS(Arg1);
    CCEVENT_DEFINE_OPERATOR_ARGS(Arg1, Arg2);
    CCEVENT_DEFINE_OPERATOR_ARGS(Arg1, Arg2, Arg3);
    CCEVENT_DEFINE_OPERATOR_ARGS(Arg1, Arg2, Arg3, Arg4);
#undef CCEVENT_DEFINE_OPERATOR_ARGS

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

void CCEventTest();

// #include "CCEvent.inl"

////////////////////////////////////////////////////////////////////////////////
// Implement CCDelegateHandler

CCDelegateHandler::CCDelegateHandler()
{
}

CCDelegateHandler::CCDelegateHandler(const HandlerType& handler)
{
    _delegateBase = handler;
}

void CCDelegateHandler::disable()
{
    CCDelegateBase::Ptr delegateBase = _delegateBase.lock();
    if (delegateBase)
    {
        delegateBase->setEnabledStatus(false);
    }
}

bool CCDelegateHandler::enable()
{
    CCDelegateBase::Ptr delegateBase = _delegateBase.lock();
    if (delegateBase)
    {
        delegateBase->setEnabledStatus(true);
        return true;
    }
    return false;
}

bool CCDelegateHandler::enabled() const
{
    if (_delegateBase.expired())
    {
        return false;
    }
    return _delegateBase.lock()->getEnabledStatus();
}

CCDelegateBase::Ptr CCDelegateHandler::getDelegateBase() const
{
    return (_delegateBase.expired()) ? 
        CCDelegateBase::Ptr() : _delegateBase.lock();
}

bool CCDelegateHandler::operator==(const CCDelegateHandler& other) const
{
    CCDelegateBase::Ptr l(_delegateBase.lock());
    CCDelegateBase::Ptr r(other._delegateBase.lock());
    return l == r;
}

bool CCDelegateHandler::operator!=(const CCDelegateHandler& other) const
{
    return !(*this == other);
}

bool CCDelegateHandler::operator<(const CCDelegateHandler& other) const
{
    CCDelegateBase::Ptr l(_delegateBase.lock());
    CCDelegateBase::Ptr r(other._delegateBase.lock());
    return l < r;
}

void CCDelegateHandler::swap(CCDelegateHandler &other)
{
    ::std::swap(_delegateBase, other._delegateBase);
}

////////////////////////////////////////////////////////////////////////////////
// Implement CCEvent

template <
    typename Signature,
    typename GroupType,
    typename Combiner,
    typename Interrupter
>
CCEvent<Signature, GroupType, Combiner,Interrupter>::CCEvent()
    : _raising(false)
{
}

template <
    typename Signature,
    typename GroupType,
    typename Combiner,
    typename Interrupter
>
CCDelegateHandler CCEvent<Signature, GroupType, Combiner,Interrupter>::add(::std::shared_ptr<Delegate>& delegate)
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

template <
    typename Signature,
    typename GroupType,
    typename Combiner,
    typename Interrupter
>
CCDelegateHandler CCEvent<Signature, GroupType, Combiner,Interrupter>::add(const DelegateFunction& func,
    CCDelegateAtPosition atPosition = AT_BACK)
{
    auto delegate = ::std::make_shared<Delegate>(func, false,  atPosition);
    return add(delegate);
}

template <
    typename Signature,
    typename GroupType,
    typename Combiner,
    typename Interrupter
>
CCDelegateHandler CCEvent<Signature, GroupType, Combiner,Interrupter>::add(const GroupType& group,
    const DelegateFunction& func,
    CCDelegateAtPosition atPosition = AT_BACK)
{
    auto delegate = ::std::make_shared<Delegate>(func, true,  atPosition);
    delegate->group = group;
    return add(delegate);
}

template <
    typename Signature,
    typename GroupType,
    typename Combiner,
    typename Interrupter
>
void CCEvent<Signature, GroupType, Combiner,Interrupter>::remove(const CCDelegateHandler& handler)
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

template <
    typename Signature,
    typename GroupType,
    typename Combiner,
    typename Interrupter
>
typename ::std::function<Signature>::result_type
CCEvent<Signature, GroupType, Combiner,Interrupter>::operator () ()
{
    CCDelegateInvoke<DelegateFunction, ResultType, Combiner, Interrupter> invoker;
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

#endif /* __FUNDATION__CCEVENT_H__ */
