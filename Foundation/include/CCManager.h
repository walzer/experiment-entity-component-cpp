#ifndef __FOUNDATION__CCMANAGER_H__
#define __FOUNDATION__CCMANAGER_H__

class CCContext;

class CCManager
{
public:
    virtual ~CCManager();

    virtual bool init(CCContext *) = 0;
    virtual void done(CCContext *) = 0;
};

#endif  // __FOUNDATION__CCMANAGER_H__
