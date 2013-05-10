#include "CCEvent.h"

using namespace std;

class System
{
public:
    CCEvent<System, int> event;

    void run()
    {
        event.raise(this, 3);
    }
    static void scdf(System* sender, const int& arg)
    {
        printf("%d %s\n", __LINE__, __FUNCTION__);
    }
    void cdf(System* sender, const int& arg)
    {
        printf("%d %s\n", __LINE__, __FUNCTION__);
    }
};

void cf(System* sender, const int& arg)
{
    printf("%d %s\n", __LINE__, __FUNCTION__);
}

void test1();

void CCEventTest()
{
    //printf("%d %s\n", __LINE__, __FUNCTION__);
    //System sys;
    //sys.event.add(cf);

    //sys.event.add(System::scdf, "f2");
    //auto f3 = std::bind(&System::cdf, sys, std::placeholders::_1, std::placeholders::_2);
    //sys.event.add(f3);
    //sys.run();
    //auto d = sys.event.get("f2");
    //printf("%d %s\n", __LINE__,d->target_type().name());
    //d = sys.event.get(typeid(f3).name());
    //printf("%d %s\n", __LINE__,d->target_type().name());
    //sys.event.remove("f2");
    //sys.run();
    //printf("-------------------------------------\n");
    test1();
    //printf("%d %s\n", __LINE__, __FUNCTION__);
}

class System1
{
public:
    CCEvent1<void> event;

    void run()
    {
        event();
    }
    static void scdf()
    {
        printf("%d %s\n", __LINE__, __FUNCTION__);
    }
    void cdf()
    {
        printf("%d %s\n", __LINE__, __FUNCTION__);
    }
};

CCEvent1<void> event;
CCDelegateHandler handler[4];
void cf0()
{
    printf("%d %s\n", __LINE__, __FUNCTION__);
}
void cf1()
{
    printf("%d %s\n", __LINE__, __FUNCTION__);
}
bool doRemove = false;
int removeIdx = 0;
void cf2()
{
    if (doRemove)
    {
        event.remove(handler[3+removeIdx]);
    }
    printf("%d %s\n", __LINE__, __FUNCTION__);
}
void cf3()
{
    printf("%d %s\n", __LINE__, __FUNCTION__);
}
void cf4()
{
    printf("%d %s\n", __LINE__, __FUNCTION__);
}
void removeAll()
{
    for(int i = 0; i < 5; ++i) event.remove(handler[i]);
}
void test1()
{
    printf("%d %s\n", __LINE__, __FUNCTION__);

    printf("test forward call\n");
    handler[0] = event.add(cf0);
    handler[1] = event.add(cf1);
    handler[2] = event.add(cf2);
    handler[3] = event.add(cf3);
    handler[4] = event.add(cf4);
    event();
    printf("test remove handler 2\n");
    event.remove(handler[2]);
    event();
    removeAll();

    printf("test backward call\n");
    handler[0] = event.add(&cf0, AT_FRONT); // vs2012 need add & first.
    handler[1] = event.add(cf1, AT_FRONT);
    handler[2] = event.add(cf2, AT_FRONT);
    handler[3] = event.add(cf3, AT_FRONT);
    handler[4] = event.add(cf4, AT_FRONT);
    event();
    removeAll();

    printf("test forward grouped call\n");
    handler[4] = event.add(0, cf0);
    handler[0] = event.add(1, cf1);
    handler[1] = event.add(1, cf2);
    handler[2] = event.add(1, cf3);
    handler[3] = event.add(2, cf4);
    event();
    removeAll();

    printf("test backward grouped call\n");
    handler[4] = event.add(2, cf0, AT_FRONT);
    handler[0] = event.add(1, cf1, AT_FRONT);
    handler[1] = event.add(1, cf2, AT_FRONT);
    handler[2] = event.add(1, cf3, AT_FRONT);
    handler[3] = event.add(0, cf4, AT_FRONT);
    event();
    removeAll();

    printf("%d %s\n", __LINE__, __FUNCTION__);
}