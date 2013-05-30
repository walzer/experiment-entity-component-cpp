#ifndef __MANAGER__CCRENDER_MANAGER_H__
#define __MANAGER__CCRENDER_MANAGER_H__

#include "CCFoundation.h"

class CCRenderComponent;
class CCVertices;

class CCRenderManager : public CCManager
{
public:
    typedef CCRenderManager ThisType;
    typedef CCManager BaseType;

    virtual ~CCRenderManager();

    virtual bool init(CCContext *context);
    virtual void done(CCContext *context);

    void addBatch(CCRenderComponent * com, const CCVec3 &pos);

protected:
    void begin();
    void draw();
    void end();

    void batchDraw();
    CCDelegateHandler beginDelegate;
    CCDelegateHandler drawDelegate;
    CCDelegateHandler postDrawDelegate;
    CCDelegateHandler endDelegate;

    ::std::vector<::std::tuple<CCRenderComponent *, CCVec3>> _batchVertices;
};

#endif  // __MANAGER__CCRENDER_MANAGER_H__
