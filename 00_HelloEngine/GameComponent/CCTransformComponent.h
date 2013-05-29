#ifndef __FOUNDATION__CCTRANSFORM_COMPONENT_H__
#define __FOUNDATION__CCTRANSFORM_COMPONENT_H__

#include "CCComponent.h"
#include "CCVector.h"

class CCTransformComponent : public CCComponent
{
public:
    typedef CCComponent BaseType;
    typedef CCTransformComponent ThisType;

    virtual bool init(CCContext *context);
    virtual void done();

    const CCVec3 &getPosition()
    {
        return _position;
    }
    void setPosition(const CCVec3 &pos)
    {
        _position = pos;
    }
    CCVec3 _position;
};

#endif  // __FOUNDATION__CCTRANSFORM_COMPONENT_H__
