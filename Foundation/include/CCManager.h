#ifndef __FOUNDATION_CC__TO_STRINGMANAGER_H__
#define __FOUNDATION_CC__TO_STRINGMANAGER_H__

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

#endif  // __FOUNDATION_CC__TO_STRINGMANAGER_H__
