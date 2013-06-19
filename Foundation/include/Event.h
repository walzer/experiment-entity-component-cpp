#ifndef __FOUNDATION__CC_EVENT_H__
#define __FOUNDATION__CC_EVENT_H__

#include "Delegate.h"

namespace cc {;

template <
    typename Signature
>
class Event : private Uncopyable {
public:
    typedef Event<Signature> ThisType;
    typedef function<Signature> FunctionType;
    typedef typename FunctionType::result_type ResultType;
    typedef Delegate<FunctionType> DelegateType;
    typedef list<DelegateType> DelegateListType;
    typedef typename DelegateListType::iterator DelegateIteratorType;
    //Event()
    //    : _raising(false) {
    //}

    void pushBack(const FunctionType &function) {
        _delegates.emplace_back(function);
        return _afterInsert((++_delegates.rbegin()).base());
    }
    void pushBack(Signature *pfn) {
        assert(pfn);
        _delegates.emplace_back(FunctionType(pfn), pfn);
        return _afterInsert((++_delegates.rbegin()).base());
    }
    void pushBack(const FunctionType &function, const void *pUniqueAddress);
    template <
        typename TargetType
    >
    void pushBack(ResultType (TargetType:: *pmemfn)(), TargetType *target);
    template <
        typename TargetType
    >
    void pushBack(ResultType (TargetType:: *pmemfn)(), const shared_ptr<TargetType> &target);

    //DelegateHandler pushFront(void *pfn, const FunctionType &function);
    void find(void *pfn);
    void remove(void *pfn);
    void clear();

private:

    void _afterInsert(const DelegateIteratorType &iter) {
        _address.emplace(iter->getAddress(), iter);
    }

    //bool _raising;
    //DelegateListType::iterator _raisingIter;
    DelegateListType _delegates;
    unordered_map<const void *, const DelegateIteratorType> _address;
};

void eventTest();
////////////////////////////////////////////////////////////////////////////////

class DelegateHandler {
public:
    typedef ::std::weak_ptr<CCDelegateBase> HandlerType;

    inline DelegateHandler();
    inline DelegateHandler(const HandlerType& handler);

    inline void disable();
    inline bool enable();
    inline bool enabled() const;
    inline CCDelegateBase::Ptr getDelegateBase() const;

    inline bool operator==(const DelegateHandler& other) const;
    inline bool operator!=(const DelegateHandler& other) const;
    inline bool operator<(const DelegateHandler& other) const;
    inline void swap(DelegateHandler &other);

private:
    HandlerType _delegateBase;
};

template <
    typename Signature,
    typename GroupType = int,
    typename Combiner = _UseLastValue,
    typename Interrupter = _InvokeNoneInterrupt
>
class Event2
{
    // Uncopyable
    Event2(const Event2 &);
    Event2 &operator = (const Event2 &);
public:
    typedef Event2<Signature, GroupType> ThisType;
    typedef ::std::function<Signature> DelegateFunction;
    typedef typename DelegateFunction::result_type  ResultType;
    typedef ::std::list<::std::shared_ptr<CCDelegateBase>> DelegateList;
    typedef CCDelegate<DelegateFunction, GroupType, DelegateList::iterator> Delegate;
    
    Event2();

    DelegateHandler add(::std::shared_ptr<Delegate>& delegate);
    DelegateHandler add(const DelegateFunction& func,
        CCDelegateAtPosition atPosition = AT_BACK);
    DelegateHandler add(const GroupType& group,
        const DelegateFunction& func,
        CCDelegateAtPosition atPosition = AT_BACK);

    void remove(const DelegateHandler& handler);

    ResultType operator () ();

#define CCEVENT_DEFINE_OPERATOR_ARGS(...) \
    template< CC_TYPES_WITH_TYPENAME(__VA_ARGS__) > \
    ResultType operator () (CC_TYPES_APPEND_PARAS(__VA_ARGS__)) \
    { \
        CCDelegateInvoke<DelegateFunction, ResultType, Combiner, Interrupter> invoker; \
        _raising = true; \
        auto end = _frontList.end(); \
        auto groupEnd = _groupedLists.end(); \
        Delegate * delegate = nullptr; \
        for (_raisingIter = _frontList.begin(); _raisingIter != end; ) \
        { \
            delegate = (Delegate*)(*_raisingIter++).get(); \
            if (delegate->getEnabledStatus()) \
            { \
                if (invoker.invoke(delegate->function, CC_TYPES_TO_PARAS(__VA_ARGS__))) \
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
                    if (invoker.invoke(delegate->function, CC_TYPES_TO_PARAS(__VA_ARGS__))) \
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
                if (invoker.invoke(delegate->function, CC_TYPES_TO_PARAS(__VA_ARGS__))) \
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

}   // namespace cc

void CCEvent2Test();

// #include "Event2.inl"

namespace cc {;

////////////////////////////////////////////////////////////////////////////////
// Implement DelegateHandler

DelegateHandler::DelegateHandler()
{
}

DelegateHandler::DelegateHandler(const HandlerType& handler)
{
    _delegateBase = handler;
}

void DelegateHandler::disable()
{
    CCDelegateBase::Ptr delegateBase = _delegateBase.lock();
    if (delegateBase)
    {
        delegateBase->setEnabledStatus(false);
    }
}

bool DelegateHandler::enable()
{
    CCDelegateBase::Ptr delegateBase = _delegateBase.lock();
    if (delegateBase)
    {
        delegateBase->setEnabledStatus(true);
        return true;
    }
    return false;
}

bool DelegateHandler::enabled() const
{
    if (_delegateBase.expired())
    {
        return false;
    }
    return _delegateBase.lock()->getEnabledStatus();
}

CCDelegateBase::Ptr DelegateHandler::getDelegateBase() const
{
    return (_delegateBase.expired()) ? 
        CCDelegateBase::Ptr() : _delegateBase.lock();
}

bool DelegateHandler::operator==(const DelegateHandler& other) const
{
    CCDelegateBase::Ptr l(_delegateBase.lock());
    CCDelegateBase::Ptr r(other._delegateBase.lock());
    return l == r;
}

bool DelegateHandler::operator!=(const DelegateHandler& other) const
{
    return !(*this == other);
}

bool DelegateHandler::operator<(const DelegateHandler& other) const
{
    CCDelegateBase::Ptr l(_delegateBase.lock());
    CCDelegateBase::Ptr r(other._delegateBase.lock());
    return l < r;
}

void DelegateHandler::swap(DelegateHandler &other)
{
    ::std::swap(_delegateBase, other._delegateBase);
}

////////////////////////////////////////////////////////////////////////////////
// Implement Event2

template <
    typename Signature,
    typename GroupType,
    typename Combiner,
    typename Interrupter
>
Event2<Signature, GroupType, Combiner,Interrupter>::Event2()
    : _raising(false)
{
}

template <
    typename Signature,
    typename GroupType,
    typename Combiner,
    typename Interrupter
>
DelegateHandler Event2<Signature, GroupType, Combiner,Interrupter>::add(::std::shared_ptr<Delegate>& delegate)
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
    return DelegateHandler(delegate);
}

template <
    typename Signature,
    typename GroupType,
    typename Combiner,
    typename Interrupter
>
DelegateHandler Event2<Signature, GroupType, Combiner,Interrupter>::add(const DelegateFunction& func,
    CCDelegateAtPosition atPosition)
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
DelegateHandler Event2<Signature, GroupType, Combiner,Interrupter>::add(const GroupType& group,
    const DelegateFunction& func,
    CCDelegateAtPosition atPosition)
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
void Event2<Signature, GroupType, Combiner,Interrupter>::remove(const DelegateHandler& handler)
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
Event2<Signature, GroupType, Combiner,Interrupter>::operator () ()
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

}   // namespace cc

#endif /* __FOUNDATION__CC_EVENT_H__ */
