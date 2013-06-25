#include "pch.h"

#include "Event.h"

#define REPEAT for (int i = 0; i < 100; ++i)

namespace cc {;

using namespace std;

void eventProfile() {
    printf("\n%d %s\n\n", __LINE__, __FUNCTION__);

    static const int MAX_NUM = 10000;
    unique_ptr<char[]> address(new char[MAX_NUM]);

    Event<void(), _UseLastValue, int, less<int>> ve;
    function<void()> vDoSomeThing = [](){
        static unsigned profileId;
        profileId = ProfileStart(profileId, "vDoSomeThing");
        ProfileStop(profileId);
    };
    for (int i = 0; i < MAX_NUM; ++i) {
        ve.push(vDoSomeThing, address.get() + i);
    }
    REPEAT{
        ProfileFunction(ve.raise(), "ve::raise()");
    }
    ve.removeAll();
    ////////////////////////////////////////////////////////////////////////////
    Event<int(), _UseLastValue, int, less<int>> ie;
    function<int()> iDoSomeThing = [](){
        static unsigned profileId;
        profileId = ProfileStart(profileId, "iDoSomeThing");
        ProfileStop(profileId);
        return 1;
    };
    int reti;
    for (int i = 0; i < MAX_NUM; ++i) {
        ie.push(iDoSomeThing, address.get() + i);
    }
    REPEAT{
        ProfileFunction(reti = ie.raise(), "ie::raise()");
    }
    cout << "ie::raise() return: " << reti << endl;

    printf("\n%d %s\n\n", __LINE__, __FUNCTION__);
}
////////////////////////////////////////////////////////////////////////////////

#define MEM_FN(ret, name) \
    ret name() { printf("call(%d) %s()\n", id, __FUNCTION__); return ret(); } \
    ret name##i(int i1) { printf("call(%d) %s(%d)\n", id, __FUNCTION__, i1); return ret(); } \
    ret name##ii(int i1, int i2) { printf("call(%d) %s(%d, %d)\n", id, __FUNCTION__, i1, i2); return ret(); } \
    ret name##iii(int i1, int i2, int i3) { printf("call(%d) %s(%d, %d, %d)\n", id, __FUNCTION__, i1, i2, i3); return ret(); } \
    ret name##iiii(int i1, int i2, int i3, int i4) { printf("call(%d) %s(%d, %d, %d, %d)\n", id, __FUNCTION__, i1, i2, i3, i4); return ret(); }

#define STATIC_FN(ret, name) \
    static ret name() { printf("call %s()\n", __FUNCTION__); return ret(); } \
    static ret name##i(int i1) { printf("call %s(%d)\n", __FUNCTION__, i1); return ret(); } \
    static ret name##ii(int i1, int i2) { printf("call %s(%d, %d)\n", __FUNCTION__, i1, i2); return ret(); } \
    static ret name##iii(int i1, int i2, int i3) { printf("call %s(%d, %d, %d)\n", __FUNCTION__, i1, i2, i3); return ret(); } \
    static ret name##iiii(int i1, int i2, int i3, int i4) { printf("call %s(%d, %d, %d, %d)\n", __FUNCTION__, i1, i2, i3, i4); return ret(); }

struct EventFunctions {
    EventFunctions(int i) : id(i){}
    int id;

    MEM_FN(void, vfn)
    STATIC_FN(void, svfn)

    MEM_FN(int, ifn)
    STATIC_FN(int, sifn)

    // bind functions
    static void tobindvfn() { printf("call bindvfn()\n"); }
    static function<void()> bindvfn;
} efs1(1), efs2(2), efs3(3), efs4(4), efs5(5), efs6(6), efs7(7), efs8(8);

function<void()> EventFunctions::bindvfn = bind(&EventFunctions::tobindvfn);

void eventTest()
{
    printf("\n%d %s\n\n", __LINE__, __FUNCTION__);

    Event<void(), _UseLastValue, int, less<int>> ev;
    auto evUntil = [](){return true;};
    printf("test Event<void()> common function\n");
    ev.push(EventFunctions::bindvfn);
    ev.push(EventFunctions::bindvfn, &EventFunctions::bindvfn);
    ev.push(EventFunctions::svfn);
    ev.push(&EventFunctions::vfn, &efs1);
    ev.push(2, &EventFunctions::vfn, &efs2);
    ev.push(2, &EventFunctions::vfn, &efs3);
    ev.push(4, &EventFunctions::vfn, &efs5);
    ev.push(4, &EventFunctions::vfn, &efs6);
    //ev.push(&VoidFunction::func, make_shared<VoidFunction>(6));
    printf("Event<void()>::raise:\n");
    ev.raise();
    printf("Event<void()>::raise group:\n");
    ev.raise(2);
    printf("Event<void()>::raise until:\n");
    ev.raiseUntil(evUntil);
    printf("Event<void()>::raise group until:\n");
    ev.raiseUntil(2, evUntil);
    ev.removeAll();

    printf("test push order(8-1):\n");
    ev.push(&EventFunctions::vfn, &efs8);
    ev.push(&EventFunctions::vfn, &efs7);
    //ev.push(&EventFunctions::vfn, &efs1);

    ev.push(3, &EventFunctions::vfn, &efs6);
    ev.push(3, &EventFunctions::vfn, &efs5);
    ev.push(2, &EventFunctions::vfn, &efs4);
    ev.push(2, &EventFunctions::vfn, &efs3);
    ev.push(1, &EventFunctions::vfn, &efs2);
    ev.push(1, &EventFunctions::vfn, &efs1);

    ev.raise(); // should output 2 1 4 3 6 5 8 7
    ev.removeAll();

    printf("test push order(1-8):\n");
    ev.push(&EventFunctions::vfn, &efs1, DelegatePosition::AT_FRONT);
    ev.push(&EventFunctions::vfn, &efs2, DelegatePosition::AT_FRONT);
    //ev.push(&EventFunctions::vfn, &efs1);

    ev.push(1, &EventFunctions::vfn, &efs3, DelegatePosition::AT_FRONT);
    ev.push(1, &EventFunctions::vfn, &efs4, DelegatePosition::AT_FRONT);
    ev.push(2, &EventFunctions::vfn, &efs5, DelegatePosition::AT_FRONT);
    ev.push(2, &EventFunctions::vfn, &efs6, DelegatePosition::AT_FRONT);
    ev.push(3, &EventFunctions::vfn, &efs7, DelegatePosition::AT_FRONT);
    ev.push(3, &EventFunctions::vfn, &efs8, DelegatePosition::AT_FRONT);

    ev.raise(); // shoud output 2 1 4 3 6 5 8 7
    ev.removeAll();

    printf("test push and front(GbUbUb GfUfUf Gb:123 456 7):\n");
    ev.push(0, &EventFunctions::vfn, &efs1);
    ev.push(&EventFunctions::vfn, &efs2);
    ev.push(&EventFunctions::vfn, &efs3);

    ev.push(0, &EventFunctions::vfn, &efs4, DelegatePosition::AT_FRONT);
    ev.push(&EventFunctions::vfn, &efs5, DelegatePosition::AT_FRONT);
    ev.push(&EventFunctions::vfn, &efs6, DelegatePosition::AT_FRONT);

    ev.push(0, &EventFunctions::vfn, &efs7);

    ev.raise(); // should output 6 5 4 1 7 2 3
    ev.removeAll();

    {
        Event<void(), _UseLastValue, int, greater<int>> ev;
        printf("test group compare by greater (1-5):\n");
        ev.push(&EventFunctions::vfn, &efs1, DelegatePosition::AT_FRONT);
        ev.push(2, &EventFunctions::vfn, &efs2, DelegatePosition::AT_FRONT);
        ev.push(2, &EventFunctions::vfn, &efs3);
        ev.push(1, &EventFunctions::vfn, &efs4);
        ev.push(&EventFunctions::vfn, &efs5);

        ev.raise(); // should output 1 2 3 4 5
        ev.removeAll();
    }

    printf("test varing deletion:\n");
    DelegateHandler hBindvfn = ev.push(EventFunctions::bindvfn);
    ev.push(EventFunctions::bindvfn, &EventFunctions::bindvfn);
    ev.push(EventFunctions::svfn, DelegatePosition::AT_FRONT);
    ev.push(&EventFunctions::vfn, &efs1, DelegatePosition::AT_FRONT);
    ev.push(1, &EventFunctions::vfn, &efs2);
    ev.push(1, &EventFunctions::vfn, &efs3);
    ev.push(2, &EventFunctions::vfn, &efs4);
    ev.push(2, &EventFunctions::vfn, &efs5);

    ev.raise();
    printf("remove EventFunctions::bindvfn:\n");
    ev.remove(&EventFunctions::bindvfn);
    ev.raise();
    printf("remove by disable handler:\n");
    hBindvfn.disable();
    ev.raise();
    printf("remove group 1:\n");
    ev.remove(&efs3);
    ev.remove(&efs2);
    ev.raise();
    printf("remove front ungrouped:\n");
    ev.remove(DelegatePosition::AT_FRONT);
    ev.raise();
    printf("remove group 2:\n");
    ev.remove(2);
    ev.raise();

    int val;
    Event<int()> ei;
    auto eiUntil = [](int){return true;};
    printf("test Event<int()> common function\n");
    ei.push(EventFunctions::sifn);
    ei.push(&EventFunctions::ifn, &efs1);
    ei.push(2, &EventFunctions::ifn, &efs2);
    ei.push(2, &EventFunctions::ifn, &efs3);
    ei.push(4, &EventFunctions::ifn, &efs5);
    ei.push(4, &EventFunctions::ifn, &efs6);
    //ev.push(&VoidFunction::func, make_shared<VoidFunction>(6));
    printf("Event<int()>::raise:\n");
    val = ei.raise();
    printf("Event<int()>::raise group:\n");
    val = ei.raise(2);
    printf("Event<int()>::raise until:\n");
    val = ei.raiseUntil(eiUntil);
    printf("Event<int()>::raise group until:\n");
    val = ei.raiseUntil(2, eiUntil);
    ei.removeAll();

    Event<int(int), _UseLastValue, int, less<int>> eii;
    eii.push(EventFunctions::sifni);
    eii.push(&EventFunctions::ifni, &efs1);
//    eii.raise(1);
    eventProfile();
}

}   // namespace cc
