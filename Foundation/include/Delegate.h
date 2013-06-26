#ifndef __FOUNDATION__CC_DELEGATE_H__
#define __FOUNDATION__CC_DELEGATE_H__

#include "CppStl.h"

#include "FundationMacrosH.h"
#include "Uncopyable.h"

namespace cc {;

enum class DelegatePosition {
    AT_BACK,
    AT_FRONT
};

class DelegateBase {
public:
    void disable() {
        _disabled = true;
    }

    bool disabled() const {
        if (_disabled) {
            return true;
        }
        if (any_of(_trackObjs.begin(), _trackObjs.end(), [](const weak_ptr<void> &o) { return o.expired(); })) {
            _disabled = true;
        }
        return _disabled;
    }

    void track(const weak_ptr<void> &obj) {
        _trackObjs.push_back(obj);
    }

    const void *getAddress() const {
        return (_address) ? _address : this;
    }
protected:
    DelegateBase(const void *address) : _disabled(false), _address(address) {}

private:
    const void *_address;
    mutable bool _disabled;
    vector<weak_ptr<void>> _trackObjs;
};

class DelegateHandler {
    typedef weak_ptr<DelegateBase> HandlerType;
public:

    DelegateHandler() {}

    DelegateHandler(const HandlerType& handler)
        : _handler(handler){}

    DelegateHandler &disable() {
        shared_ptr<DelegateBase> delegateBase = _handler.lock();
        if (delegateBase) {
            delegateBase->disable();
        }
        return *this;
    }

    bool disabled() const {
        if (_handler.expired())
        {
            return true;
        }
        return _handler.lock()->disabled();
    }

    DelegateHandler &track(const weak_ptr<void> &obj) {
        if (!disabled()) {
            _handler.lock()->track(obj);
        }
        return *this;
    }

    bool operator ==(const DelegateHandler& other) const {
        shared_ptr<DelegateBase> l(_handler.lock());
        shared_ptr<DelegateBase> r(other._handler.lock());
        return l == r;
    }

    bool operator !=(const DelegateHandler& other) const {
        return !(*this == other);
    }

    bool operator <(const DelegateHandler& other) const {
        shared_ptr<DelegateBase> l(_handler.lock());
        shared_ptr<DelegateBase> r(other._handler.lock());
        return l < r;
    }

    void swap(DelegateHandler &other) {
        ::cc::swap(_handler, other._handler);
    }

private:
    HandlerType _handler;
};

enum class _DelegateCategory {
    AT_FRONT,
    GROUPED,
    AT_BACK
};

template <typename Group>
struct _DelegateGroupKey {
    typedef pair<_DelegateCategory, Group> type;
};

template <
    typename Group, 
    typename GroupCompare
>
class _DelegateGroupKeyLess {
public:
    _DelegateGroupKeyLess() {}

    _DelegateGroupKeyLess(const GroupCompare &compare)
        : _compare(compare) {}

    bool operator ()(
        const typename _DelegateGroupKey<Group>::type &key1,
        const typename _DelegateGroupKey<Group>::type &key2
    ) {
        if (key1.first != key2.first ) return key1.first < key2.first;
        return _compare(key1.second, key2.second);
    }
private:
    GroupCompare _compare;
};

template <
    typename FunctionType,
    typename GroupKey
>
class Delegate : public DelegateBase {
public:
    Delegate(
        const void *address,
        const GroupKey &key,
        const FunctionType &function
    ) :
    DelegateBase(address),
    _function(function),
    _groupKey(key) {}

    const FunctionType &getFunction() const {
        return _function;
    }

    const GroupKey &getGroupKey() const {
        return _groupKey;
    }

private:
    FunctionType _function;
    GroupKey _groupKey;
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

template <typename ResultType>
struct _DelegateResultCheck {
    typedef bool (type)(ResultType);
};

template <>
struct _DelegateResultCheck<void> {
    typedef bool(type)();
};

template <
    typename ResultType,
    typename FunctionType,
    typename Combiner
>
class _DelegateInvoke {
public:
    typedef typename _DelegateResultCheck<ResultType>::type CheckSignature;
    ResultType getCombinedResult() {
        return _combinedResult;
    }

    bool checkResult(const function<CheckSignature> &function) {
        return function(_result);
    }

    void operator () (const FunctionType &function) {
        _result = function();
        _combinedResult = _combiner(_combinedResult, _result);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Define member functors deal with 1-4 arguments.
#define CC__DELEGATE_INVOKE_OPERATOR_WITH_ARGS(...) \
    template <CC_TYPES_WITH_TYPENAME(__VA_ARGS__)> \
    void operator () ( \
        const FunctionType &function, \
        CC_TYPES_APPEND_PARA(__VA_ARGS__) \
    ) { \
        _result = function(CC_TYPES_TO_PARA(__VA_ARGS__)); \
        _combinedResult = _combiner(_combinedResult, _result); \
    }

    CC__DELEGATE_INVOKE_OPERATOR_WITH_ARGS(Arg1);
    CC__DELEGATE_INVOKE_OPERATOR_WITH_ARGS(Arg1, Arg2);
    CC__DELEGATE_INVOKE_OPERATOR_WITH_ARGS(Arg1, Arg2, Arg3);
    CC__DELEGATE_INVOKE_OPERATOR_WITH_ARGS(Arg1, Arg2, Arg3, Arg4);
#undef CC__DELEGATE_INVOKE_OPERATOR_WITH_ARGS
    ////////////////////////////////////////////////////////////////////////////

private:
    ResultType _result;
    ResultType _combinedResult;
    Combiner   _combiner;
};

template <
    typename FunctionType,
    typename Combiner
>
class _DelegateInvoke<void, FunctionType, Combiner> {
public:
    void getCombinedResult() {}

    bool checkResult(const function<bool()> &function) {
        return function();
    }

    void operator () (const FunctionType& function) {
        function();
    }

    ////////////////////////////////////////////////////////////////////////////
    // Define member functors deal with 1-4 arguments.
#define CC__DELEGATE_INVOKE_OPERATOR_WITH_ARGS(...) \
    template <CC_TYPES_WITH_TYPENAME(__VA_ARGS__)> \
    void operator () ( \
        const FunctionType &function, \
        CC_TYPES_APPEND_PARA(__VA_ARGS__) \
    ) { \
        function(CC_TYPES_TO_PARA(__VA_ARGS__)); \
    }

    CC__DELEGATE_INVOKE_OPERATOR_WITH_ARGS(Arg1);
    CC__DELEGATE_INVOKE_OPERATOR_WITH_ARGS(Arg1, Arg2);
    CC__DELEGATE_INVOKE_OPERATOR_WITH_ARGS(Arg1, Arg2, Arg3);
    CC__DELEGATE_INVOKE_OPERATOR_WITH_ARGS(Arg1, Arg2, Arg3, Arg4);
#undef CC__DELEGATE_INVOKE_OPERATOR_WITH_ARGS
    ////////////////////////////////////////////////////////////////////////////
};

}   // namespace cc

#endif  // __FOUNDATION__CC_DELEGATE_H__
