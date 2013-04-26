#include "CCEvent.h"

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

void CCEventTest()
{
    printf("%d %s\n", __LINE__, __FUNCTION__);
    System sys;
    sys.event.add(cf);
    sys.event.add(System::scdf, "f2");
    auto f3 = std::bind(&System::cdf, sys, std::placeholders::_1, std::placeholders::_2);
    sys.event.add(f3);
    sys.run();
    auto d = sys.event.get("f2");
    printf("%d %s\n", __LINE__,d->target_type().name());
    d = sys.event.get(typeid(f3).name());
    printf("%d %s\n", __LINE__,d->target_type().name());
    sys.event.remove("f2");
    sys.run();
    printf("%d %s\n", __LINE__, __FUNCTION__);
}