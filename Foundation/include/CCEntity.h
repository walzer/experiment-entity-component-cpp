#ifndef __FOUNDATION_CC__TO_STRINGENTITY_H__
#define __FOUNDATION_CC__TO_STRINGENTITY_H__

#include <map>
#include <vector>

#include "CCComponent.h"
#include "String.h"

class CCContext;

class CCEntity : public CCComponent
{
public:
    typedef CCComponent BaseType;
    typedef CCEntity ThisType;
    typedef ::std::shared_ptr<CCEntity> Ptr;
    static const Ptr NullPtr;

    virtual bool init();
    virtual void done();

    virtual void update(float dt);

    void addComponent(const CCComponent::Ptr& com);
    CCComponent::Ptr findComponent(const String& name);

    template <typename ComponentType>
    ::std::shared_ptr<ComponentType> findComponent(const String& name)
    {
        return ::std::static_pointer_cast<ComponentType>(findComponent(name));
    }
    void removeComponent(const String& name);

    void addChild(const Ptr& child);
    const Ptr& findChild(const String& name);
    void removeChild(const Ptr& child);

    inline const Ptr& getParent() const;
    void setParent(const Ptr& parent);

    CCContext *getContext()
    {
        return _context;
    }
    void setContext(CCContext *context)
    {
        _context = context;
    }
protected:
    ::std::map<String, CCComponent::Ptr> _components;
    ::std::vector<Ptr> _children;
    Ptr _parent;
    CCContext *_context;
};

// include "CCEntity.inl"
const CCEntity::Ptr& CCEntity::getParent() const
{
    return _parent;
}

#endif  // __FOUNDATION_CC__TO_STRINGENTITY_H__
