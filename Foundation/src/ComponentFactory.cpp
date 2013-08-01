#include "pch.h"

#include "ComponentFactory.h"

#include "IComponent.h"

namespace cc {;

ComponentFactory::ComponentFactory() {}

shared_ptr<IComponent> ComponentFactory::create(
    const CachedString &type,
    const IDom *args
) {
    auto iter = _infos.find(type);
    if (iter != _infos.end()) {
        const function<shared_ptr<IComponent>()> &create = iter->second->create;
        if (create) {
            auto ret = create();
            ret->init(args);
            return ret;
        }
    }
    return shared_ptr<IComponent>();
}

const ComponentTypeInfo *ComponentFactory::findTypeInfo(
    const CachedString &type
) {
    auto iter = _infos.find(type);
    if (iter != _infos.end()) {
        return iter->second;
    }
    return nullptr;
}

bool ComponentFactory::isDependencesComplete() {
    // TODO(yangws): Implement component dependences check method.
    return true;
}

ComponentFactory *ComponentFactory::registerComponent(
    const ComponentTypeInfo &info
) {
    _infos[CC_CSTR(info.name)] = &info;
    return this;
}

ComponentFactory *ComponentFactory::unRegisterComponent(
    const CachedString &type
) {
    auto iter = _infos.find(type);
    if (iter != _infos.end()) {
        _infos.erase(iter);
    }
    return this;
}

ComponentFactory *ComponentFactory::getInstance() {
    static ComponentFactory instance; 
    return &instance;
}

}   // namespace cc
