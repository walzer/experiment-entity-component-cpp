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
    typename Combiner = _UseLastValue,
    typename Group = int,
    typename GroupCompare = less<Group>
>
class Event : private Uncopyable {
    typedef Event<Signature, Combiner, Group, GroupCompare> ThisType;
    typedef function<Signature> FunctionType;
    typedef typename FunctionType::result_type ResultType;
    typedef typename _DelegateGroupKey<Group>::type GroupKey;
    typedef typename _DelegateResultCheck<ResultType>::type CheckSignature;
    typedef _DelegateGroupKeyLess<Group, GroupCompare> GroupKeyLess;
    typedef Delegate<FunctionType, GroupKey> DelegateType;
    typedef list<shared_ptr<DelegateType>> DelegateListType;
    typedef typename DelegateListType::iterator DelegateIteratorType;
public:

    DelegateHandler push(
        const FunctionType &function, 
        const DelegatePosition pos = DelegatePosition::AT_BACK
    ) {
        assert(function);
        return _push(function, nullptr, pos);
    }

    DelegateHandler push(
        const FunctionType &function,
        const void *address,
        const DelegatePosition pos = DelegatePosition::AT_BACK
    ) {
        assert(function && address);
        return _push(function, address, pos);
    }

    DelegateHandler push(
        Signature *pfn,
        const DelegatePosition pos = DelegatePosition::AT_BACK
    ) {
        assert(pfn);
        function<Signature> function(pfn);
        return _push(function, (const void *)pfn, pos);
    }

    template <typename TargetType>
    DelegateHandler push(
        ResultType (TargetType:: *pmemfn)(),
        TargetType *target,
        const DelegatePosition pos = DelegatePosition::AT_BACK
    ) {
        assert(pmemfn && target);
        function<Signature> function = bind(pmemfn, target);
        return _push(function, target, pos);
    }

    template <
        typename TargetType,
        typename Arg1
    >
    DelegateHandler push(
        ResultType (TargetType:: *pmemfn)(Arg1),
        TargetType *target,
        const DelegatePosition pos = DelegatePosition::AT_BACK
    ) {
        assert(pmemfn && target);
        function<Signature> function = bind(pmemfn, target, placeholders::_1);
        return _push(function, target, pos);
    }
    //template <
    //    typename TargetType
    //>
    //void push(ResultType (TargetType:: *pmemfn)(), const shared_ptr<TargetType> &target) {
    //}

    DelegateHandler push(
        const Group &group,
        const FunctionType &function,
        const DelegatePosition pos = DelegatePosition::AT_BACK
    ) {
        assert(function);
        return _push(group, function, nullptr, pos);
    }

    DelegateHandler push(
        const Group &group,
        const FunctionType &function,
        const void *address,
        const DelegatePosition pos = DelegatePosition::AT_BACK
    ) {
        assert(function && address);
        return _push(group, function, address, pos);
    }

    DelegateHandler push(const Group &group,
        Signature *pfn,
        const DelegatePosition pos = DelegatePosition::AT_BACK
    ) {
        assert(pfn);
        function<Signature> function(pfn);
        return _push(group, function, (const void *)pfn, pos);
    }

    template <typename TargetType>
    DelegateHandler push(
        const Group &group,
        ResultType (TargetType:: *pmemfn)(),
        TargetType *target,
        const DelegatePosition pos = DelegatePosition::AT_BACK
    ) {
        assert(pmemfn && target);
        function<Signature> function = bind(pmemfn, target);
        return _push(group, function, target, pos);
    }

    //template <
    //    typename TargetType
    //>
    //void push(ResultType (TargetType:: *pmemfn)(), const shared_ptr<TargetType> &target) {
    //}

    void remove(void *address) {
        auto addressIter = _address.find(address);
        if (addressIter != _address.end()) {
            (*addressIter->second)->disable();
        }
    }

    void remove(const DelegatePosition &pos) {
        GroupKey key(
            (pos == DelegatePosition::AT_BACK) ?
            _DelegateCategory::AT_BACK : _DelegateCategory::AT_FRONT,
            Group()
        );
        _removeGroup(key);
    }

    void remove(const Group & group) {
        GroupKey key(_DelegateCategory::GROUPED, group);
        _removeGroup(key);
    }

    void removeAll() {
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

    ResultType raise(const Group &group) {
        _DelegateInvoke<ResultType, FunctionType, Combiner> invoke;
        GroupKey key(_DelegateCategory::GROUPED, group);
        auto mapEnd = _groups.end();
        auto mapIter = _groups.find(key);
        if (mapIter != mapEnd) {
            auto iter = mapIter->second;
            mapIter = _groups.upper_bound(key);
            auto end = (mapIter == mapEnd) ? _delegates.end() : mapIter->second;
            for ( ; iter != end; ) {
                if ((*iter)->disabled()) {
                    _removeDelegate(iter++);
                    continue;
                }
                invoke((*iter++)->getFunction());
            }
        }
        return invoke.getCombinedResult();
    }
    
    ResultType raiseUntil(const function<CheckSignature>  &function) {
        _DelegateInvoke<ResultType, FunctionType, Combiner> invoke;
        auto end = _delegates.end();
        for (auto iter = _delegates.begin(); iter != end; ) {
            if ((*iter)->disabled()) {
                _removeDelegate(iter++);
                continue;
            }
            invoke((*iter++)->getFunction());
            if (invoke.checkResult(function)) {
                break;
            }
        }
        return invoke.getCombinedResult();
    }

    ResultType raiseUntil(
        const Group &group,
        const function<CheckSignature>  &function
    ) {
        _DelegateInvoke<ResultType, FunctionType, Combiner> invoke;
        GroupKey key(_DelegateCategory::GROUPED, group);
        auto mapEnd = _groups.end();
        auto mapIter = _groups.find(key);
        if (mapIter != mapEnd) {
            auto iter = mapIter->second;
            mapIter = _groups.upper_bound(key);
            auto end = (mapIter == mapEnd) ? _delegates.end() : mapIter->second;
            for ( ; iter != end; ) {
                if ((*iter)->disabled()) {
                    _removeDelegate(iter++);
                    continue;
                }
                invoke((*iter++)->getFunction());
                if (invoke.checkResult(function)) {
                    break;
                }
            }
        }
        return invoke.getCombinedResult();
    }

private:
    bool _groupKeyEqual(const GroupKey &key1, const GroupKey &key2) {
        if (_groupKeyLess(key1, key2)) return false;
        if (_groupKeyLess(key2, key1)) return false;
        return true;
    }

    DelegateHandler _push(
        const FunctionType &function,
        const void *address,
        const DelegatePosition pos
    ) {
        shared_ptr<DelegateType> delegate;
        if (pos == DelegatePosition::AT_BACK) {
            GroupKey key(_DelegateCategory::AT_BACK, Group());
            delegate = make_shared<DelegateType>(address, key, function);
            _pushBack(key, delegate);
        } else {
            GroupKey key(_DelegateCategory::AT_FRONT, Group());
            delegate = make_shared<DelegateType>(address, key, function);
            _pushFront(key, delegate);
        }
        return DelegateHandler(delegate);
    }

    DelegateHandler _push(
        const Group &group,
        const FunctionType &function,
        const void *address,
        const DelegatePosition pos
    ) {
        shared_ptr<DelegateType> delegate;
        if (pos == DelegatePosition::AT_BACK) {
            GroupKey key(_DelegateCategory::GROUPED, group);
            delegate = make_shared<DelegateType>(address, key, function);
            _pushBack(key, delegate);
        } else {
            GroupKey key(_DelegateCategory::GROUPED, group);
            delegate = make_shared<DelegateType>(address, key, function);
            _pushFront(key, delegate);
        }
        return DelegateHandler(delegate);
    }

    void _pushBack(
        const GroupKey &key,
        const shared_ptr<DelegateType> &delegate
    ) {
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
    }

    void _pushFront(
        const GroupKey &key,
        const shared_ptr<DelegateType> &delegate
    ) {
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
