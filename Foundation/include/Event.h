#ifndef __FOUNDATION__CC_EVENT_H__
#define __FOUNDATION__CC_EVENT_H__

#include "Delegate.h"

namespace cc {;

template <
    typename Signature,
    typename Combiner
>
class TestEvent;

template <
    typename Signature,
    typename Combiner = _UseLastValue
>
class Event : private Uncopyable {
public:
    typedef Event<Signature, Combiner> ThisType;
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

    void pushBack(const FunctionType &function, const void *pUniqueAddress) {
        assert(pUniqueAddress);
        _delegates.emplace_back(function, pUniqueAddress);
        return _afterInsert((++_delegates.rbegin()).base());
    }

    void pushBack(Signature *pfn) {
        assert(pfn);
        _delegates.emplace_back(FunctionType(pfn), pfn);
        return _afterInsert((++_delegates.rbegin()).base());
    }

    template <
        typename TargetType
    >
    void pushBack(ResultType (TargetType:: *pmemfn)(), TargetType *target) {
        _delegates.emplace_back(bind(pmemfn, target), target);
        return _afterInsert((++_delegates.rbegin()).base());
    }

    template <
        typename TargetType
    >
    void pushBack(ResultType (TargetType:: *pmemfn)(), const shared_ptr<TargetType> &target);

    //DelegateHandler pushFront(void *pfn, const FunctionType &function);
    void find(void *address);
    void remove(void *address);
    void clear() {
        _delegates.clear();
        _address.clear();
    }

    ResultType raise() {
        _DelegateInvoke<ResultType, FunctionType, Combiner> invoke;
        auto end = _delegates.end();
        for (auto iter = _delegates.begin(); iter != end; ) {
            if (iter->disabled()) {
                _removeDelegate(iter++);
                continue;
            }
            invoke(iter++->function);
        }
        return invoke.getCombinedResult();
    }


private:
    friend class TestEvent<Signature, Combiner>;

    void _afterInsert(const DelegateIteratorType &iter) {
        _address.emplace(iter->getAddress(), iter);
    }

    void _removeDelegate(const DelegateIteratorType &iter) {
        _address.erase(iter->getAddress());
        _delegates.erase(iter);
    }
    //bool _raising;
    //DelegateListType::iterator _raisingIter;
    DelegateListType _delegates;
    unordered_map<const void *, const DelegateIteratorType> _address;
};

void eventTest();

}   // namespace cc

#endif /* __FOUNDATION__CC_EVENT_H__ */
