#ifndef __FUNDATION__CCMANAGER_H__
#define __FUNDATION__CCMANAGER_H__

class CCContext;

class CCManager
{
public:
    virtual ~CCManager() {}

    virtual bool init(CCContext *) = 0;
    virtual void done(CCContext *) = 0;
};

#endif  // __FUNDATION__CCMANAGER_H__
