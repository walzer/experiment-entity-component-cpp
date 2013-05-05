#ifndef __FUNDATION__CCCOMPONENT_H__
#define __FUNDATION__CCCOMPONENT_H__

#include <functional>
#include <map>
#include <memory>

#include "CCString.h"

class CCContext;
class CCEntity;

class CCComponent
{
public:
    typedef CCComponent     ThisType;
    typedef ::std::shared_ptr<CCComponent> Ptr;

    CCComponent();
    virtual ~CCComponent();

    virtual CCString getName();

public:
    typedef Ptr (CreatorSignature)();
    typedef ::std::function<CreatorSignature>   Creator;
    typedef ::std::map<CCString, Creator>   CreatorEntry;

    static Ptr create(const CCString& typeName);
    static CreatorEntry& getCreatorEntry();
    static void registerCreator(const CCString& typeName, const Creator& creator);
private:
    CCEntity* _owner;
};

template < typename ComponentType >
struct CCComponentRegister
{
    CCComponentRegister(const CCString& typeName)
    {
        auto creator = []()->CCComponent::Ptr
        {
            return ::std::make_shared<typename ComponentType>();
        };
        CCComponent::registerCreator(typeName, creator);
    }
    CCComponentRegister(const CCString& typeName, const CCComponent::Creator& creator)
    {
        CCComponent::registerCreator(typeName, creator);
    }
};

#define CCCOMPONENT_REGISTER_CREATOR \
    _CCCOMPONENT_REGISTER_CREATOR_IMPL(IMPLEMENT_CLASS)

#define _CCCOMPONENT_REGISTER_CREATOR_IMPL(typeName) \
    static CCComponentRegister<typeName> CCGLUE(s_registerCreator, typeName)(CCTYPE_TO_STRING(typeName))

#define CCCOMPONENT_REGISTER_CUSTOM_CREATOR(creator) \
    _CCCOMPONENT_REGISTER_CUSTOM_CREATOR_IMPL(IMPLEMENT_CLASS, creator)

#define _CCCOMPONENT_REGISTER_CUSTOM_CREATOR_IMPL(typeName) \
    static CCComponentRegister<typeName> CCGLUE(s_registerCreator, typeName)(CCTYPE_TO_STRING(typeName), creator)

#endif  // __FUNDATION__CCCOMPONENT_H__
