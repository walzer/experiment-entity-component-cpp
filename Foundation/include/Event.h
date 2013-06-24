#ifndef __FOUNDATION__CC_EVENT_H__
#define __FOUNDATION__CC_EVENT_H__

#include "Delegate.h"

namespace cc {;

template <
    typename Signature,
    typename Combiner,// = _UseLastValue,
    typename Group,// = int,
    typename GroupCompare// = less<Group>
>
class Event : private Uncopyable {
public:
    typedef Event<Signature, Combiner, Group, GroupCompare> ThisType;
    typedef function<Signature> FunctionType;
    typedef typename FunctionType::result_type ResultType;
    typedef typename _DelegateGroupKey<Group>::type GroupKey;
    typedef _DelegateGroupKeyLess<Group, GroupCompare> GroupKeyLess;
    typedef Delegate<FunctionType, GroupKey> DelegateType;
    typedef list<DelegateType> DelegateListType;
    typedef typename DelegateListType::iterator DelegateIteratorType;

    //Event()
    //    : _raising(false) {
    //}

    void pushBack(const FunctionType &function) {
        GroupKey key(_DelegateCategory::AT_BACK, Group());
        return _afterInsert(_pushBackDelegate(key), nullptr, key, function);
    }

    void pushBack(const FunctionType &function, const void *pUniqueAddress) {
        assert(pUniqueAddress);
        GroupKey key(_DelegateCategory::AT_BACK, Group());
        return _afterInsert(_pushBackDelegate(key), pUniqueAddress, key, function);
    }

    void pushBack(Signature *pfn) {
        assert(pfn);
        GroupKey key(_DelegateCategory::AT_BACK, Group());
        function<Signature> function(pfn);
        return _afterInsert(_pushBackDelegate(key), (const void *)pfn, key, function);
    }

    template <
        typename TargetType
    >
    void pushBack(ResultType (TargetType:: *pmemfn)(), TargetType *target) {
        assert(pmemfn && target);
        GroupKey key(_DelegateCategory::AT_BACK, Group());
        function<Signature> function = bind(pmemfn, target);
        return _afterInsert(_pushBackDelegate(key), target, key, function);
    }

    //template <
    //    typename TargetType
    //>
    //void pushBack(ResultType (TargetType:: *pmemfn)(), const shared_ptr<TargetType> &target) {
    //}

    void pushBack(const Group &group, const FunctionType &function) {
        GroupKey key(_DelegateCategory::GROUPED, group);
        return _afterInsert(_pushBackDelegate(key), nullptr, key, function);
    }

    void pushBack(const Group &group, const FunctionType &function, const void *pUniqueAddress) {
        assert(pUniqueAddress);
        GroupKey key(_DelegateCategory::GROUPED, group);
        return _afterInsert(_pushBackDelegate(key), pUniqueAddress, key, function);
    }

    void pushBack(const Group &group, Signature *pfn) {
        assert(pfn);
        GroupKey key(_DelegateCategory::GROUPED, group);
        function<Signature> function(pfn);
        return _afterInsert(_pushBackDelegate(key), pfn, key, function);
    }

    template <
        typename TargetType
    >
    void pushBack(const Group &group, ResultType (TargetType:: *pmemfn)(), TargetType *target) {
        assert(pmemfn && target);
        GroupKey key(_DelegateCategory::GROUPED, group);
        function<Signature> function = bind(pmemfn, target);
        return _afterInsert(_pushBackDelegate(key), target, key, function);
    }

    //template <
    //    typename TargetType
    //>
    //void pushBack(ResultType (TargetType:: *pmemfn)(), const shared_ptr<TargetType> &target) {
    //}

    void pushFront(const FunctionType &function) {
        GroupKey key(_DelegateCategory::AT_FRONT, Group());
        return _afterInsert(_pushFrontDelegate(key), nullptr, key, function);
    }

    void pushFront(const FunctionType &function, const void *pUniqueAddress) {
        assert(pUniqueAddress);
        GroupKey key(_DelegateCategory::AT_FRONT, Group());
        return _afterInsert(_pushFrontDelegate(key), pUniqueAddress, key, function);
    }

    void pushFront(Signature *pfn) {
        assert(pfn);
        GroupKey key(_DelegateCategory::AT_FRONT, Group());
        function<Signature> function(pfn);
        return _afterInsert(_pushFrontDelegate(key), pfn, key, function);
    }

    template <
        typename TargetType
    >
    void pushFront(ResultType (TargetType:: *pmemfn)(), TargetType *target) {
        assert(pmemfn && target);
        GroupKey key(_DelegateCategory::AT_FRONT, Group());
        function<Signature> function = bind(pmemfn, target);
        return _afterInsert(_pushFrontDelegate(key), target, key, function);
    }

    //template <
    //    typename TargetType
    //>
    //void pushFront(ResultType (TargetType:: *pmemfn)(), const shared_ptr<TargetType> &target) {
    //}

    void pushFront(const Group &group, const FunctionType &function) {
        GroupKey key(_DelegateCategory::GROUPED, group);
        return _afterInsert(_pushFrontDelegate(key), nullptr, key, function);
    }

    void pushFront(const Group &group, const FunctionType &function, const void *pUniqueAddress) {
        assert(pUniqueAddress);
        GroupKey key(_DelegateCategory::GROUPED, group);
        return _afterInsert(_pushFrontDelegate(key), pUniqueAddress, key, function);
    }

    void pushFront(const Group &group, Signature *pfn) {
        assert(pfn);
        GroupKey key(_DelegateCategory::GROUPED, group);
        function<Signature> function(pfn);
        return _afterInsert(_pushFrontDelegate(key), pfn, key, function);
    }

    template <
        typename TargetType
    >
    void pushFront(const Group &group, ResultType (TargetType:: *pmemfn)(), TargetType *target) {
        assert(pmemfn && target);
        GroupKey key(_DelegateCategory::GROUPED, group);
        function<Signature> function = bind(pmemfn, target);
        return _afterInsert(_pushFrontDelegate(key), target, key, function);
    }

    //template <
    //    typename TargetType
    //>
    //void pushFront(ResultType (TargetType:: *pmemfn)(), const shared_ptr<TargetType> &target) {
    //}

    //DelegateHandler pushFront(void *pfn, const FunctionType &function);
    void find(void *address);
    void remove(void *address);
    void clear() {
        _delegates.clear();
        _address.clear();
        _groups.clear();
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
    void _afterInsert(
        const DelegateIteratorType &iter,
        const void *address,
        const GroupKey &key,
        const FunctionType &function) {
        iter->setAddress(address);
        iter->groupKey = key;
        iter->function = function;
        _address.insert(make_pair(address, iter));
    }

    bool _groupKeyEqual(const GroupKey &key1, const GroupKey &key2) {
        if (_groupKeyLess(key1, key2)) return false;
        if (_groupKeyLess(key2, key1)) return false;
        return true;
    }

    const DelegateIteratorType _pushBackDelegate(const GroupKey &key) {
        auto mapIter = (key.first == _DelegateCategory::AT_BACK) ? 
            _groups.end() : _groups.upper_bound(key);

        auto listIter = (mapIter == _groups.end()) ?
            _delegates.end() : mapIter->second;
        auto newIter = _delegates.insert(listIter, DelegateType());

        mapIter = _groups.lower_bound(key);
        if (mapIter == _groups.end() || !_groupKeyEqual(mapIter->first, key)) {
            _groups.insert(make_pair(key, newIter));
        }
        return newIter;
    }

    const DelegateIteratorType _pushFrontDelegate(const GroupKey &key) {
        auto mapIter = (key.first == _DelegateCategory::AT_FRONT) ? 
            _groups.begin() : _groups.lower_bound(key);

        auto listIter = (mapIter == _groups.end()) ?
            _delegates.end() : mapIter->second;
        auto newIter = _delegates.insert(listIter, DelegateType());

        if (mapIter != _groups.end() && _groupKeyEqual(mapIter->first, key)) {
            _groups.erase(key);
        }
        _groups.insert(make_pair(key, newIter));
        return newIter;
    }

    void _removeDelegate(const DelegateIteratorType &iter) {
        _address.erase(iter->getAddress());
        _delegates.erase(iter);
    }
    //bool _raising;
    //DelegateListType::iterator _raisingIter;
    DelegateListType _delegates;
    unordered_map<const void *, const DelegateIteratorType> _address;
    map<GroupKey, const DelegateIteratorType, GroupKeyLess> _groups;
    GroupKeyLess _groupKeyLess;
};

void eventTest();

}   // namespace cc

#endif /* __FOUNDATION__CC_EVENT_H__ */
