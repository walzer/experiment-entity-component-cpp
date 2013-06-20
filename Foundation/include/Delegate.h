#ifndef __FOUNDATION__CC_DELEGATE_H__
#define __FOUNDATION__CC_DELEGATE_H__

#include "CppStl.h"

#include "FundationMacrosH.h"
#include "Uncopyable.h"

namespace cc {;

class DelegateBase : public enable_shared_from_this<DelegateBase> {
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
    DelegateBase() : _disabled(false), _address(nullptr) {}
    DelegateBase(const void *addr) : _disabled(false), _address(addr) {}

private:
    mutable bool _disabled;
    const void *_address;
    vector<weak_ptr<void>> _trackObjs;
};

template <
    typename FunctionType
>
class Delegate : public DelegateBase {
public:
    Delegate(const FunctionType &fun) : function(fun) {}
    Delegate(const FunctionType &fun, const void *addr) : function(fun), DelegateBase(addr) {}

    FunctionType function;
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

template <
    typename ResultType,
    typename FunctionType,
    typename Combiner
>
class _DelegateInvoke {
public:
    ResultType getCombinedResult() {
        return _combinedResult;
    }

    ResultType getResult() {
        return _result;
    }

    void operator () (const FunctionType &function) {
        _result = function();
        _combinedResult = _combiner(_combinedResult, _result);
    }
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

    void getResult() {}

    void operator () (const FunctionType& function) {
        function();
    }
};

}   // namespace cc

#endif  // __FOUNDATION__CC_DELEGATE_H__
