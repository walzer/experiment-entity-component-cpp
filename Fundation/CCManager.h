#ifndef __FUNDATION__CCMANAGER_H__
#define __FUNDATION__CCMANAGER_H__

class CCManager
{
public:
    typedef CCManager                       ThisType;

    virtual ~CCManager();
    virtual bool init();
    virtual void done();

    void update();

private:
};

#endif  // __FUNDATION__CCMANAGER_H__
