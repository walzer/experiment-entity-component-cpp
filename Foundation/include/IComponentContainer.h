#ifndef __FOUNDATION__I_COMPONENT_CONTAINER_H__
#define __FOUNDATION__I_COMPONENT_CONTAINER_H__

#include "IComponent.h"

namespace cc {;

class IComponentContainer : public IComponent {
public:
    virtual void add(IComponent *com) = 0;
    virtual IComponent *findByName(const CachedString &name) = 0;
    virtual vector<IComponent *> findByType(const CachedString &type) = 0;
    virtual void remove(IComponent *com) = 0;
};

}   // namespace cc

#endif  // __FOUNDATION__I_COMPONENT_CONTAINER_H__
