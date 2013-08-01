#ifndef __FOUNDATION__COMPONPENT_FACTORY_H__
#define __FOUNDATION__COMPONPENT_FACTORY_H__

#include "CppStl.h"
#include "Uncopyable.h"

namespace cc {;

class IComponent;
struct ComponentTypeInfo;
class IDom;

class ComponentFactory: public Uncopyable {
public:
    shared_ptr<IComponent> create(
        const CachedString &type,
        const IDom *args = nullptr
    );

    const ComponentTypeInfo *findTypeInfo(const CachedString &type);

    bool isDependencesComplete();

    ComponentFactory *registerComponent(const ComponentTypeInfo &info);

    ComponentFactory *unRegisterComponent(const CachedString &type);

    static ComponentFactory *getInstance();

private:
    ComponentFactory();
    // TODO(yangws): Remove CachedStringLess when CachedString complete.
    struct CachedStringLess {
        bool operator()(const CachedString &left, const CachedString &right) {
            return *left < *right;
        }
    };
    map<
        const CachedString,
        const ComponentTypeInfo *,
        CachedStringLess
    > _infos;
};

}   // namespace cc

#define CC_REGISTER_COMPONENT(type) \
    static auto registered##type = ::cc::ComponentFactory::getInstance()-> \
        registerComponent(type::getTypeInfo())

#endif  // __FOUNDATION__COMPONPENT_FACTORY_H__
