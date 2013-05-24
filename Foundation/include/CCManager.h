#ifndef __FOUNDATION__CCMANAGER_H__
#define __FOUNDATION__CCMANAGER_H__

class CCContext;

class CCManager
{
    // Uncopyable
    CCManager(const CCManager &);
    CCManager &operator = (const CCManager &);
public:
    CCManager();
    virtual ~CCManager();

    virtual bool init(CCContext *) = 0;
    virtual void done(CCContext *) = 0;
};

#endif  // __FOUNDATION__CCMANAGER_H__
