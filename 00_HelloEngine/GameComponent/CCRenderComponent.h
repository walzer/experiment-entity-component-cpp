#ifndef __FOUNDATION__CCRENDER_COMPONENT_H__
#define __FOUNDATION__CCRENDER_COMPONENT_H__

#include "CCFoundation.h"

class CCRenderManager;
class CCTransformComponent;
class CCVertices;

class CCRenderComponent : public CCComponent
{
public:
    typedef CCComponent BaseType;
    typedef CCRenderComponent ThisType;

    virtual bool init(CCContext* context);
    virtual void done();

    void setVertices(const ::std::shared_ptr<CCVertices> &vertices)
    {
        _vertices = vertices;
    }

    const ::std::shared_ptr<CCVertices> &getVertices()
    {
        return _vertices;
    }

protected:
    CCDelegateHandler drawEventHandler;
    CCRenderManager *_manager;
    CCTransformComponent *_transformComponent;
    ::std::shared_ptr<CCVertices> _vertices;
};

#endif  // __FOUNDATION__CCRENDER_COMPONENT_H__
