#include "CCEvent.h"

#define GLUE_VAR(n, var) var##n ( n )

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

CCDelegateHandler handler[5];

template <typename EventType>
void removeAll(EventType& event)
{
    printf("removeAll\n");
    for(int i = 0; i < 5; ++i) event.remove(handler[i]); 
}

void test1()
{

    printf("%d %s\n", __LINE__, __FUNCTION__);

    CCEvent1<void()> event;
    struct VoidFunction
    {
        VoidFunction(int i) : id(i){}
        int id;

        void operator () ()
        {
            printf("call VoidFunction %d\n", id);
        }
    };
    VoidFunction CCFOR_EACH_NUM(GLUE_VAR, vf, vf, vf, vf, vf);

    printf("test forward call\n");
    handler[0] = event.add(vf1);
    handler[1] = event.add(vf2);
    handler[2] = event.add(vf3);
    handler[3] = event.add(vf4);
    handler[4] = event.add(vf5);
    event();
    printf("test remove handler 3\n");
    event.remove(handler[2]);
    event();
    removeAll(event);

    printf("test backward call\n");
    handler[0] = event.add(vf1, AT_FRONT); // vs2012 need add & first.
    handler[1] = event.add(vf2, AT_FRONT);
    handler[2] = event.add(vf3, AT_FRONT);
    handler[3] = event.add(vf4, AT_FRONT);
    handler[4] = event.add(vf5, AT_FRONT);
    event();
    removeAll(event);

    printf("test forward grouped call\n");
    handler[4] = event.add(0, vf1);
    handler[0] = event.add(1, vf2);
    handler[1] = event.add(1, vf3);
    handler[2] = event.add(1, vf4);
    handler[3] = event.add(2, vf5);
    event();
    removeAll(event);

    printf("test backward grouped call\n");
    handler[4] = event.add(2, vf1, AT_FRONT);
    handler[0] = event.add(1, vf2, AT_FRONT);
    handler[1] = event.add(1, vf3, AT_FRONT);
    handler[2] = event.add(1, vf4, AT_FRONT);
    handler[3] = event.add(0, vf5, AT_FRONT);
    event();
    removeAll(event);

    printf("test remove when raise event\n");
    handler[0] = event.add(vf1);
    handler[1] = event.add(vf2);
    handler[2] = event.add(bind(removeAll<decltype(event)>, ref(event)));
    handler[3] = event.add(vf4);
    handler[4] = event.add(vf5);
    event();
    removeAll(event);

    ///////////////////////////////////////////////////////////////
    CCEvent1<int()> event1;
    struct IntFunction
    {
        IntFunction(int i) : id(i){}
        int id;

        int operator () ()
        {
            printf("call IntFunction %d\n", id);
            return id;
        }
    };
    IntFunction CCFOR_EACH_NUM(GLUE_VAR, intf, intf, intf, intf, intf);

    printf("test int function call\n");
    handler[0] = event1.add(intf1);
    handler[1] = event1.add(intf2);
    handler[2] = event1.add(intf3);
    handler[3] = event1.add(intf4);
    handler[4] = event1.add(intf5);
    int ret = event1();
    printf("event1 return %d\n", ret);
    printf("%d %s\n", __LINE__, __FUNCTION__);
}