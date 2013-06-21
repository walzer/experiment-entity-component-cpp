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
        _delegates.push_back(DelegateType());
        auto iter = (++_delegates.rbegin()).base();
        iter->function = function;
        return _afterInsert(iter);
    }

    void pushBack(const FunctionType &function, const void *pUniqueAddress) {
        assert(pUniqueAddress);
        _delegates.push_back(DelegateType());
        auto iter = (++_delegates.rbegin()).base();
        iter->function = function;
        iter->setAddress(pUniqueAddress);
        return _afterInsert(iter);
    }

    void pushBack(Signature *pfn) {
        assert(pfn);
        _delegates.push_back(DelegateType());
        auto iter = (++_delegates.rbegin()).base();
        iter->function = FunctionType(pfn);
        iter->setAddress((const void *)pfn);
        return _afterInsert(iter);
    }

    template <
        typename TargetType
    >
    void pushBack(ResultType (TargetType:: *pmemfn)(), TargetType *target) {
        assert(pmemfn && target);
        _delegates.push_back(DelegateType());
        auto iter = (++_delegates.rbegin()).base();
        iter->function = bind(pmemfn, target);
        iter->setAddress(target);
        return _afterInsert(iter);
    }

    //template <
    //    typename TargetType
    //>
    //void pushBack(ResultType (TargetType:: *pmemfn)(), const shared_ptr<TargetType> &target);

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
        _address.insert(make_pair(iter->getAddress(), iter));
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
