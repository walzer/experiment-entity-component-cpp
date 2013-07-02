#ifndef __FOUNDATION__I_COMPONENT_H__
#define __FOUNDATION__I_COMPONENT_H__

#include "CppStl.h"

namespace cc {;

class IComponentContainer;
class IDom;

class IComponent : private enable_shared_from_this<IComponent> {
public:
    IComponent()
    : _owner(nullptr)
    , _enabled(false) {
    }

    virtual ~IComponent() {}

    virtual void init(const IDom &arguments) = 0;
    virtual void done() = 0;
    virtual const CachedString &getTypeName() = 0;

    const CachedString &getName() const {
        return _name;
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

    void setOwner(const IComponentContainer *owner) {
        _owner = owner;
    }

private:
    CachedString _name;
    IComponentContainer *_owner;
    bool _enabled;
};

}   // namespace cc

#endif  // __FOUNDATION__I_COMPONENT_H__
