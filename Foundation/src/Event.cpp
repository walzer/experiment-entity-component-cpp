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
        ve.pushBack(vDoSomeThing, address.get() + i);
    }
    REPEAT{
        ProfileFunction(ve.raise(), "ve::raise()");
    }
    ve.clear();
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
        ie.pushBack(iDoSomeThing, address.get() + i);
    }
    REPEAT{
        ProfileFunction(reti = ie.raise(), "ie::raise()");
    }
    cout << "ie::raise() return: " << reti << endl;

    printf("\n%d %s\n\n", __LINE__, __FUNCTION__);
}
////////////////////////////////////////////////////////////////////////////////

#define MEM_FN(ret, name) \
    ret name##() { printf("call(%d) %s()\n", id, __FUNCTION__); return ret(); } \
    ret name##i(int i1) { printf("call(%d) %s(%d)\n", id, __FUNCTION__, i1); return ret(); } \
    ret name##ii(int i1, int i2) { printf("call(%d) %s(%d, %d)\n", id, __FUNCTION__, i1, i2); return ret(); } \
    ret name##iii(int i1, int i2, int i3) { printf("call(%d) %s(%d, %d, %d)\n", id, __FUNCTION__, i1, i2, i3); return ret(); } \
    ret name##iiii(int i1, int i2, int i3, int i4) { printf("call(%d) %s(%d, %d, %d, %d)\n", id, __FUNCTION__, i1, i2, i3, i4); return ret(); }

#define STATIC_FN(ret, name) \
    static ret name##() { printf("call %s()\n", __FUNCTION__); return ret(); } \
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
    static void tobindvfn() { printf("call(%d) bindvfn()\n"); }
    static function<void()> bindvfn;
} efs1(1), efs2(2), efs3(3), efs4(4), efs5(5), efs6(6), efs7(7), efs8(8), efs9(9), efs10(10);

function<void()> EventFunctions::bindvfn = bind(&EventFunctions::tobindvfn);

void eventTest()
{
    printf("\n%d %s\n\n", __LINE__, __FUNCTION__);

    Event<void(), _UseLastValue, int, less<int>> ev;
    printf("test varing insert delegate\n");
    ev.pushBack(EventFunctions::bindvfn);
    ev.pushBack(EventFunctions::bindvfn, &EventFunctions::bindvfn);
    ev.pushBack(EventFunctions::svfn);
    ev.pushBack(&EventFunctions::vfn, &efs1);
    //ev.pushBack(&VoidFunction::func, make_shared<VoidFunction>(6));

    ev.raise();
    ev.clear();

    printf("test pushback order(8-1):\n");
    ev.pushBack(&EventFunctions::vfn, &efs8);
    ev.pushBack(&EventFunctions::vfn, &efs7);
    //ev.pushBack(&EventFunctions::vfn, &efs1);

    ev.pushBack(3, &EventFunctions::vfn, &efs6);
    ev.pushBack(3, &EventFunctions::vfn, &efs5);
    ev.pushBack(2, &EventFunctions::vfn, &efs4);
    ev.pushBack(2, &EventFunctions::vfn, &efs3);
    ev.pushBack(1, &EventFunctions::vfn, &efs2);
    ev.pushBack(1, &EventFunctions::vfn, &efs1);

    ev.raise(); // should output 2 1 4 3 6 5 8 7
    ev.clear();

    printf("test pushfront order(1-8):\n");
    ev.pushFront(&EventFunctions::vfn, &efs1);
    ev.pushFront(&EventFunctions::vfn, &efs2);
    //ev.pushBack(&EventFunctions::vfn, &efs1);

    ev.pushFront(1, &EventFunctions::vfn, &efs3);
    ev.pushFront(1, &EventFunctions::vfn, &efs4);
    ev.pushFront(2, &EventFunctions::vfn, &efs5);
    ev.pushFront(2, &EventFunctions::vfn, &efs6);
    ev.pushFront(3, &EventFunctions::vfn, &efs7);
    ev.pushFront(3, &EventFunctions::vfn, &efs8);

    ev.raise(); // shoud output 2 1 4 3 6 5 8 7
    ev.clear();

    printf("test pushback and front(GbUbUb GfUfUf Gb:123 456 7):\n");
    ev.pushBack(0, &EventFunctions::vfn, &efs1);
    ev.pushBack(&EventFunctions::vfn, &efs2);
    ev.pushBack(&EventFunctions::vfn, &efs3);

    ev.pushFront(0, &EventFunctions::vfn, &efs4);
    ev.pushFront(&EventFunctions::vfn, &efs5);
    ev.pushFront(&EventFunctions::vfn, &efs6);

    ev.pushBack(0, &EventFunctions::vfn, &efs7);

    ev.raise(); // should output 6 5 4 1 7 2 3
    ev.clear();

    {
        Event<void(), _UseLastValue, int, greater<int>> ev;
        printf("test group compare by greater (1-5):\n");
        ev.pushFront(&EventFunctions::vfn, &efs1);
        ev.pushFront(2, &EventFunctions::vfn, &efs2);
        ev.pushBack(2, &EventFunctions::vfn, &efs3);
        ev.pushBack(1, &EventFunctions::vfn, &efs4);
        ev.pushBack(&EventFunctions::vfn, &efs5);

        ev.raise(); // should output 1 2 3 4 5
        ev.clear();
    }

    printf("test varing deletion:\n");
    DelegateHandler hBindvfn = ev.pushBack(EventFunctions::bindvfn);
    ev.pushBack(EventFunctions::bindvfn, &EventFunctions::bindvfn);
    ev.pushFront(EventFunctions::svfn);
    ev.pushFront(&EventFunctions::vfn, &efs1);
    ev.pushBack(1, &EventFunctions::vfn, &efs2);
    ev.pushBack(1, &EventFunctions::vfn, &efs3);
    ev.pushBack(2, &EventFunctions::vfn, &efs4);
    ev.pushBack(2, &EventFunctions::vfn, &efs5);

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
    ev.removeFrontUngrouped();
    ev.raise();
    printf("remove group 2:\n");
    ev.removeGroup(2);
    ev.raise();

    Event<int(int), _UseLastValue, int, less<int>> eii;
    eii.pushBack(EventFunctions::sifni);
    eii.pushBack(&EventFunctions::ifni, &efs1);
//    eii.raise(1);
    eventProfile();
}

}   // namespace cc
