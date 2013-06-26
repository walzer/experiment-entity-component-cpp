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
    printf("test Event<void()> common function====\n");
    ev.push(EventFunctions::bindvfn);
    ev.push(EventFunctions::bindvfn, &EventFunctions::bindvfn);
    ev.push(EventFunctions::svfn);
    ev.push(&EventFunctions::vfn, &efs1, DelegatePosition::AT_FRONT);
    ev.push(2, &EventFunctions::vfn, &efs2);
    ev.push(2, &EventFunctions::vfn, &efs3);
    ev.push(4, &EventFunctions::vfn, &efs4);
    ev.push(4, &EventFunctions::vfn, &efs5);
    //ev.push(&VoidFunction::func, make_shared<VoidFunction>(6));
    printf("Event<void()>::raise:\n");
    ev.raise();
    printf("Event<void()>::raise group:\n");
    ev.raiseGroup(2);
    printf("Event<void()>::raise until:\n");
    ev.raiseUntil(evUntil);
    printf("Event<void()>::raise group until:\n");
    ev.raiseGroupUntil(2, evUntil);
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
    printf("test Event<int()> common function====\n");
    ei.push(EventFunctions::sifn);
    ei.push(&EventFunctions::ifn, &efs1, DelegatePosition::AT_FRONT);
    ei.push(2, &EventFunctions::ifn, &efs2);
    ei.push(2, &EventFunctions::ifn, &efs3);
    ei.push(4, &EventFunctions::ifn, &efs4);
    ei.push(4, &EventFunctions::ifn, &efs5);
    //ev.push(&VoidFunction::func, make_shared<VoidFunction>(6));
    printf("Event<int()>::raise:\n");
    val = ei.raise();
    printf("Event<int()>::raise group:\n");
    val = ei.raiseGroup(2);
    printf("Event<int()>::raise until:\n");
    val = ei.raiseUntil(eiUntil);
    printf("Event<int()>::raise group until:\n");
    val = ei.raiseGroupUntil(2, eiUntil);
    ei.removeAll();

    Event<int(int)> eii;
    printf("test Event<int(int)> common function====\n");
    eii.push(EventFunctions::sifni);
    eii.push(&EventFunctions::ifni, &efs1, DelegatePosition::AT_FRONT);
    eii.push(2, &EventFunctions::ifni, &efs2);
    eii.push(2, &EventFunctions::ifni, &efs3);
    eii.push(4, &EventFunctions::ifni, &efs4);
    eii.push(4, &EventFunctions::ifni, &efs5);
    //ev.push(&VoidFunction::func, make_shared<VoidFunction>(6));
    printf("Event<int(int)>::raise:\n");
    val = eii.raise(2);
    printf("Event<int(int)>::raise group:\n");
    val = eii.raiseGroup(2, 2);
    printf("Event<int(int)>::raise until:\n");
    val = eii.raiseUntil(eiUntil, 2);
    printf("Event<int(int)>::raise group until:\n");
    val = eii.raiseGroupUntil(2, eiUntil, 2);
    eii.removeAll();

    Event<void(int)> evi;
    printf("test Event<void(int)> common function====\n");
    evi.push(EventFunctions::svfni);
    evi.push(&EventFunctions::vfni, &efs1, DelegatePosition::AT_FRONT);
    evi.push(2, &EventFunctions::vfni, &efs2);
    evi.push(2, &EventFunctions::vfni, &efs3);
    evi.push(4, &EventFunctions::vfni, &efs4);
    evi.push(4, &EventFunctions::vfni, &efs5);
    //ev.push(&VoidFunction::func, make_shared<VoidFunction>(6));
    printf("Event<int(int)>::raise:\n");
    evi.raise(2);
    printf("Event<int(int)>::raise group:\n");
    evi.raiseGroup(2, 2);
    printf("Event<int(int)>::raise until:\n");
    evi.raiseUntil(evUntil, 2);
    printf("Event<int(int)>::raise group until:\n");
    evi.raiseGroupUntil(2, evUntil, 2);
    evi.removeAll();

    Event<int(int, int, int)> eiiii;
    printf("test Event<int(int, int, int)> common function====\n");
    eiiii.push(EventFunctions::sifniii);
    eiiii.push(&EventFunctions::ifniii, &efs1, DelegatePosition::AT_FRONT);
    eiiii.push(2, &EventFunctions::ifniii, &efs2);
    eiiii.push(2, &EventFunctions::ifniii, &efs3);
    eiiii.push(4, &EventFunctions::ifniii, &efs4);
    eiiii.push(4, &EventFunctions::ifniii, &efs5);
    //ev.push(&VoidFunction::func, make_shared<VoidFunction>(6));
    printf("Event<int(int, int, int)>::raise:\n");
    eiiii.raise(2, 4, 6);
    printf("Event<int(int, int, int)>::raise group:\n");
    eiiii.raiseGroup(2, 2, 4, 6);
    printf("Event<int(int, int, int)>::raise until:\n");
    eiiii.raiseUntil(eiUntil, 2, 4, 6);
    printf("Event<int(int, int, int)>::raise group until:\n");
    eiiii.raiseGroupUntil(2, eiUntil, 2, 4, 6);
    eiiii.removeAll();

    Event<void(int, int)> evii;
    printf("test Event<void(int, int)> common function====\n");
    evii.push(EventFunctions::svfnii);
    evii.push(&EventFunctions::vfnii, &efs1, DelegatePosition::AT_FRONT);
    evii.push(2, &EventFunctions::vfnii, &efs2);
    evii.push(2, &EventFunctions::vfnii, &efs3);
    evii.push(4, &EventFunctions::vfnii, &efs4);
    evii.push(4, &EventFunctions::vfnii, &efs5);
    //ev.push(&VoidFunction::func, make_shared<VoidFunction>(6));
    printf("Event<int(int, int)>::raise:\n");
    evii.raise(2, 4);
    printf("Event<int(int, int)>::raise group:\n");
    evii.raiseGroup(2, 2, 4);
    printf("Event<int(int, int)>::raise until:\n");
    evii.raiseUntil(evUntil, 2, 4);
    printf("Event<int(int, int)>::raise group until:\n");
    evii.raiseGroupUntil(2, evUntil, 2, 4);
    evii.removeAll();

    printf("Event<void()> track weak_ptr test:\n");
    shared_ptr<void> trackedObj(new char);
    ev.push(EventFunctions::svfn).track(trackedObj);
    ev.raise();
    trackedObj.reset();
    ev.raise();
    ev.removeAll();

    eventProfile();
}

}   // namespace cc
