#ifndef __FOUNDATION__CC_EVENT_H__
#define __FOUNDATION__CC_EVENT_H__

#include "Delegate.h"

namespace cc {;

class DelegateHandler {
public:
    typedef weak_ptr<DelegateBase> HandlerType;

    DelegateHandler() {}

    DelegateHandler(const HandlerType& handler)
        : _handler(handler){}

    void disable() {
        shared_ptr<DelegateBase> delegateBase = _handler.lock();
        if (delegateBase) {
            delegateBase->disable();
        }
    }

    bool disabled() const {
        if (_handler.expired())
        {
            return true;
        }
        return _handler.lock()->disabled();
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
    typedef list<shared_ptr<DelegateType>> DelegateListType;
    typedef typename DelegateListType::iterator DelegateIteratorType;

    //Event()
    //    : _raising(false) {
    //}

    DelegateHandler pushBack(const FunctionType &function) {
        assert(function);
        GroupKey key(_DelegateCategory::AT_BACK, Group());
        return _pushBack(nullptr, key, function);
    }

    DelegateHandler pushBack(const FunctionType &function, const void *address) {
        assert(function && address);
        GroupKey key(_DelegateCategory::AT_BACK, Group());
        return _pushBack(address, key, function);
    }

    DelegateHandler pushBack(Signature *pfn) {
        assert(pfn);
        GroupKey key(_DelegateCategory::AT_BACK, Group());
        function<Signature> function(pfn);
        return _pushBack((const void *)pfn, key, function);
    }

    template <
        typename TargetType
    >
    DelegateHandler pushBack(ResultType (TargetType:: *pmemfn)(), TargetType *target) {
        assert(pmemfn && target);
        GroupKey key(_DelegateCategory::AT_BACK, Group());
        function<Signature> function = bind(pmemfn, target);
        return _pushBack(target, key, function);
    }

    template <
        typename TargetType,
        typename Arg1
    >
    DelegateHandler pushBack(ResultType (TargetType:: *pmemfn)(Arg1), TargetType *target) {
        assert(pmemfn && target);
        GroupKey key(_DelegateCategory::AT_BACK, Group());
        function<Signature> function = bind(pmemfn, target, placeholders::_1);
        return _pushBack(target, key, function);
    }
    //template <
    //    typename TargetType
    //>
    //void pushBack(ResultType (TargetType:: *pmemfn)(), const shared_ptr<TargetType> &target) {
    //}

    DelegateHandler pushBack(const Group &group, const FunctionType &function) {
        assert(function);
        GroupKey key(_DelegateCategory::GROUPED, group);
        return _pushBack(nullptr, key, function);
    }

    DelegateHandler pushBack(const Group &group, const FunctionType &function, const void *address) {
        assert(function && address);
        GroupKey key(_DelegateCategory::GROUPED, group);
        return _pushBack(address, key, function);
    }

    DelegateHandler pushBack(const Group &group, Signature *pfn) {
        assert(pfn);
        GroupKey key(_DelegateCategory::GROUPED, group);
        function<Signature> function(pfn);
        return _pushBack((const void *)pfn, key, function);
    }

    template <
        typename TargetType
    >
    DelegateHandler pushBack(const Group &group, ResultType (TargetType:: *pmemfn)(), TargetType *target) {
        assert(pmemfn && target);
        GroupKey key(_DelegateCategory::GROUPED, group);
        function<Signature> function = bind(pmemfn, target);
        return _pushBack(target, key, function);
    }

    //template <
    //    typename TargetType
    //>
    //void pushBack(ResultType (TargetType:: *pmemfn)(), const shared_ptr<TargetType> &target) {
    //}

    DelegateHandler pushFront(const FunctionType &function) {
        assert(function);
        GroupKey key(_DelegateCategory::AT_FRONT, Group());
        return _pushFront(nullptr, key, function);
    }

    DelegateHandler pushFront(const FunctionType &function, const void *address) {
        assert(function && address);
        GroupKey key(_DelegateCategory::AT_FRONT, Group());
        return _pushFront(address, key, function);
    }

    DelegateHandler pushFront(Signature *pfn) {
        assert(pfn);
        GroupKey key(_DelegateCategory::AT_FRONT, Group());
        function<Signature> function(pfn);
        return _pushFront((const void *)pfn, key, function);
    }

    template <
        typename TargetType
    >
    DelegateHandler pushFront(ResultType (TargetType:: *pmemfn)(), TargetType *target) {
        assert(pmemfn && target);
        GroupKey key(_DelegateCategory::AT_FRONT, Group());
        function<Signature> function = bind(pmemfn, target);
        return _pushFront(target, key, function);
    }

    //template <
    //    typename TargetType
    //>
    //void pushFront(ResultType (TargetType:: *pmemfn)(), const shared_ptr<TargetType> &target) {
    //}

    DelegateHandler pushFront(const Group &group, const FunctionType &function) {
        assert(function);
        GroupKey key(_DelegateCategory::GROUPED, group);
        return _pushFront(nullptr, key, function);
    }

    DelegateHandler pushFront(const Group &group, const FunctionType &function, const void *address) {
        assert(function && address);
        GroupKey key(_DelegateCategory::GROUPED, group);
        return _pushFront(address, key, function);
    }

    DelegateHandler pushFront(const Group &group, Signature *pfn) {
        assert(pfn);
        GroupKey key(_DelegateCategory::GROUPED, group);
        function<Signature> function(pfn);
        return _pushFront((const void *)pfn, key, function);
    }

    template <
        typename TargetType
    >
    DelegateHandler pushFront(const Group &group, ResultType (TargetType:: *pmemfn)(), TargetType *target) {
        assert(pmemfn && target);
        GroupKey key(_DelegateCategory::GROUPED, group);
        function<Signature> function = bind(pmemfn, target);
        return _pushFront(target, key, function);
    }

    //template <
    //    typename TargetType
    //>
    //void pushFront(ResultType (TargetType:: *pmemfn)(), const shared_ptr<TargetType> &target) {
    //}

    //DelegateHandler pushFront(void *pfn, const FunctionType &function);
    void find(void *address);
    void remove(void *address) {
        auto addressIter = _address.find(address);
        if (addressIter != _address.end()) {
            (*addressIter->second)->disable();
        }
    }

    void removeBackUngrouped() {
        GroupKey key(_DelegateCategory::AT_BACK, Group());
        _removeGroup(key);
    }

    void removeFrontUngrouped() {
        GroupKey key(_DelegateCategory::AT_FRONT, Group());
        _removeGroup(key);
    }

    void removeGroup(const Group & group) {
        GroupKey key(_DelegateCategory::GROUPED, group);
        _removeGroup(key);
    }

    void clear() {
        _delegates.clear();
        _address.clear();
        _groups.clear();
    }

    ResultType raise() {
        _DelegateInvoke<ResultType, FunctionType, Combiner> invoke;
        auto end = _delegates.end();
        for (auto iter = _delegates.begin(); iter != end; ) {
            if ((*iter)->disabled()) {
                _removeDelegate(iter++);
                continue;
            }
            invoke((*iter++)->getFunction());
        }
        return invoke.getCombinedResult();
    }

private:
    bool _groupKeyEqual(const GroupKey &key1, const GroupKey &key2) {
        if (_groupKeyLess(key1, key2)) return false;
        if (_groupKeyLess(key2, key1)) return false;
        return true;
    }

    DelegateHandler _pushBack(
        const void *address,
        const GroupKey &key,
        const FunctionType &function
    ) {
        auto delegate = make_shared<DelegateType>(address, key, function);

        auto mapEnd = _groups.end();
        auto mapIter = (key.first == _DelegateCategory::AT_BACK) ? 
            mapEnd : _groups.upper_bound(key);

        auto listIter = (mapIter == mapEnd) ?
            _delegates.end() : mapIter->second;
        auto newIter = _delegates.insert(listIter, delegate);

        mapIter = _groups.lower_bound(key);
        if (mapIter == mapEnd || !_groupKeyEqual(mapIter->first, key)) {
            _groups.insert(make_pair(key, newIter));
        }
        _address.insert(make_pair(delegate->getAddress(), newIter));
        return DelegateHandler(delegate);
    }

    DelegateHandler _pushFront(
        const void *address,
        const GroupKey &key,
        const FunctionType &function
    ) {
        auto delegate = make_shared<DelegateType>(address, key, function);

        auto mapEnd = _groups.end();
        auto mapIter = (key.first == _DelegateCategory::AT_FRONT) ? 
            _groups.begin() : _groups.lower_bound(key);

        auto listIter = (mapIter == mapEnd) ?
            _delegates.end() : mapIter->second;
        auto newIter = _delegates.insert(listIter, delegate);

        if (mapIter != mapEnd && _groupKeyEqual(mapIter->first, key)) {
            _groups.erase(key);
        }
        _groups.insert(make_pair(key, newIter));
        _address.insert(make_pair(delegate->getAddress(), newIter));
        return DelegateHandler(delegate);
    }

    void _removeDelegate(const DelegateIteratorType &iter) {
        _address.erase((*iter)->getAddress());
        auto mapIter = _groups.find((*iter)->getGroupKey());
        if (mapIter != _groups.end() && mapIter->second == iter) {
            _groups.erase(mapIter);
        }
        _delegates.erase(iter);
    }

    void _removeGroup(const GroupKey &key) {
        auto mapIter = _groups.find(key);
        if (mapIter != _groups.end()) {
            auto bgn = mapIter->second;
            auto upper = _groups.upper_bound(key);
            auto end = (upper == _groups.end()) ? _delegates.end() : upper->second;
            for_each(bgn, end, [](const shared_ptr<DelegateType> &p) { p->disable(); });
            _groups.erase(mapIter);
        }
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
