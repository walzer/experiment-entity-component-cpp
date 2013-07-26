#ifndef __FOUNDATION__I_COMPONENT_H__
#define __FOUNDATION__I_COMPONENT_H__

#include "CppStl.h"

namespace cc {;

class IComponentContainer;
class IDom;

class IComponent: private enable_shared_from_this<IComponent> {
protected:
    virtual bool _onInit(const IDom *arguments) = 0;
    virtual void _onEnable() = 0;
    virtual void _onDisable() = 0;
    virtual void _onDone() = 0;

public:
    IComponent():
        _owner(nullptr),
        _enabled(false),
        _inited(false) {
    }

    virtual ~IComponent() {}

    bool init(const IDom *arguments = nullptr) {
        if (!_inited) {
            _inited = _onInit(arguments);
        }
        return _inited;
    }

    void enable() {
        if (_inited && !_enabled) {
            _onEnable();
            _enabled = true;
        }
    }

    void disable() {
        if (_enabled) {
            _onDisable();
            _enabled = false;
        }
    }

    void done() {
        if (_inited) {
            disable();
            _onDone();
            _inited = false;
        }
    }

    bool enabled() const {
        return _enabled;
    }

    bool inited() const {
        return _inited;
    }

    const CachedString &getName() const {
        return _name;
    }

    const CachedString &getTypeName() const {
        return _typeName;
    }

    const IComponentContainer *getOwner() const {
        return _owner;
    }

    template <typename DerivedType>
    typename remove_reference<typename remove_pointer<DerivedType>::type>::type 
    *as() {
        return static_cast<
            typename remove_reference <
                typename remove_pointer<DerivedType>::type
            >::type *
        >(this);
    }

    template <typename DerivedType>
    shared_ptr<
        typename remove_reference<
            typename remove_pointer<DerivedType>::type
        >::type
    >
    asShared() {
        return static_pointer_cast<
            typename remove_reference<
                typename remove_pointer<DerivedType>::type
            >::type
        >(this->shared_from_this());
    }

protected:
    void setName(const CachedString &name) {
        _name = name;
    }

    void setTypeName(const CachedString &typeName) {
        _typeName = typeName;
    }

    void setOwner(IComponentContainer *owner) {
        _owner = owner;
    }

private:
    CachedString _name;
    CachedString _typeName;
    IComponentContainer *_owner;
    bool _enabled;
    bool _inited;
};

class IComponentContainer: public IComponent {
public:
    virtual ~IComponentContainer() {}
    virtual void add(IComponent *com) = 0;
    virtual IComponent *findByName(const CachedString &name) = 0;
    virtual vector<IComponent *> findByTypeName(const CachedString &type) = 0;
    virtual void remove(IComponent *com) = 0;
};

}   // namespace cc

#endif  // __FOUNDATION__I_COMPONENT_H__
