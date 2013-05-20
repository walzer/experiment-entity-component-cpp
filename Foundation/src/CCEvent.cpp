#include "pch.h"

#include "CCEvent.h"

#define GLUE_VAR(n, var) var##n ( n )

using namespace std;

CCDelegateHandler handler[5];

template <typename EventType>
void removeAll(EventType& event)
{
    printf("removeAll\n\n");
    for(int i = 0; i < 5; ++i) event.remove(handler[i]); 
}

void CCEventTest()
{
    printf("%d %s\n", __LINE__, __FUNCTION__);

    CCEvent<void()> event;
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
    printf("test disable handler 3\n");
    handler[2].disable();
    event();
    printf("test enable handler 3\n");
    handler[2].enable();
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
    CCEvent<int()> event1;
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
    removeAll(event1);

    CCEvent<int(), int, std::plus<int>> event2;
    printf("test combiner for int function call\n");
    handler[0] = event2.add(intf1);
    handler[1] = event2.add(intf2);
    handler[2] = event2.add(intf3);
    handler[3] = event2.add(intf4);
    handler[4] = event2.add(intf5);
    ret = event2();
    printf("event2 return %d\n", ret);
    removeAll(event2);

    struct Equal3
    {
        bool operator()(int v) { return (v ==3) ?  true : false; }
    };

    CCEvent<int(), int, std::plus<int>, Equal3> event3;
    printf("test invoke interrupt for int function call\n");
    handler[0] = event3.add(intf1);
    handler[1] = event3.add(intf2);
    handler[2] = event3.add(intf3);
    handler[3] = event3.add(intf4);
    handler[4] = event3.add(intf5);
    ret = event3();
    printf("event3 return %d\n", ret);
    removeAll(event3);

    CCEvent<void(int)> event4;
    struct VoidFunctionInt
    {
        VoidFunctionInt(int i) : id(i){}
        int id;

        void operator() (int i)
        {
            printf("call VoidFunctionInt %d with arg1 %d\n", id, i);
        }
    };
    VoidFunctionInt CCFOR_EACH_NUM(GLUE_VAR, vfint, vfint, vfint, vfint, vfint);

    printf("test call VoidFunctionInt with 1 arg\n");
    handler[0] = event4.add(vfint1);
    handler[1] = event4.add(vfint2);
    handler[2] = event4.add(vfint3);
    handler[3] = event4.add(vfint4);
    handler[4] = event4.add(vfint5);
    event4(100);
    removeAll(event4);

    CCEvent<int(int)> event5;
    struct IntFunctionInt
    {
        IntFunctionInt(int i) : id(i){}
        int id;

        int operator() (int i)
        {
            printf("call IntFunctionInt %d with arg1 %d\n", id, i);
            return id * i;
        }
    };
    IntFunctionInt CCFOR_EACH_NUM(GLUE_VAR, intfint, intfint, intfint, intfint, intfint);

    printf("test call IntFunctionInt with 1 arg\n");
    handler[0] = event5.add(intfint1);
    handler[1] = event5.add(intfint2);
    handler[2] = event5.add(intfint3);
    handler[3] = event5.add(intfint4);
    handler[4] = event5.add(intfint5);
    printf(" int(int) event return : %d\n", event5(100));
    removeAll(event5);

    CCEvent<void(int, int)> event6;
    struct VoidFunctionIntInt
    {
        VoidFunctionIntInt(int i) : id(i){}
        int id;

        void operator() (int i, int j)
        {
            printf("call VoidFunctionIntInt %d with arg1 %d, %d\n", id, i, j);
        }
    };
    VoidFunctionIntInt CCFOR_EACH_NUM(GLUE_VAR, vfii, vfii, vfii, vfii, vfii);

    printf("test call VoidFunctionIntInt with 2 arg\n");
    handler[0] = event6.add(vfii1);
    handler[1] = event6.add(vfii2);
    handler[2] = event6.add(vfii3);
    handler[3] = event6.add(vfii4);
    handler[4] = event6.add(vfii5);
    event6(100, 200);
    removeAll(event6);

    CCEvent<int(int, int)> event7;
    struct IntFunctionIntInt
    {
        IntFunctionIntInt(int i) : id(i){}
        int id;

        int operator() (int i, int j)
        {
            printf("call IntFunctionIntInt %d with arg1 %d, %d\n", id, i, j);
            return i + j;
        }
    };
    IntFunctionIntInt CCFOR_EACH_NUM(GLUE_VAR, ifii, ifii, ifii, ifii, ifii);

    printf("test call IntFunctionIntInt with 2 arg\n");
    handler[0] = event7.add(ifii1);
    handler[1] = event7.add(ifii2);
    handler[2] = event7.add(ifii3);
    handler[3] = event7.add(ifii4);
    handler[4] = event7.add(ifii5);
    printf(" int(int, int) event return : %d\n", event7(100, 200));
    removeAll(event7);

    printf("%d %s\n", __LINE__, __FUNCTION__);
}