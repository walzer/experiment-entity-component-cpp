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

struct EventFunctions {
    EventFunctions(int i) : id(i){}
    int id;

    // void member functions
    void vf() {
        printf("call(%d) vf()\n", id);
    }
    void vfi(int i1) {
        printf("call(%d) vfi(%d)\n", id, i1);
    }
    void vfii(int i1, int i2) {
        printf("call(%d) vfii(%d, %d)\n", id, i1, i2);
    }
    void vfiii(int i1, int i2, int i3) {
        printf("call(%d) vfiii(%d, %d, %d)\n", id, i1, i2, i3);
    }
    void vfiiii(int i1, int i2, int i3, int i4) {
        printf("call(%d) vfiiii(%d, %d, %d, %d)\n", id, i1, i2, i3, i4);
    }

    // void functors
    void operator () () {
        printf("call(%d) operator()()\n", id);
    }

    // void static functions
    static void svf() {
        printf("call static svf()\n");
    }

    // bind functions
    static function<void()> bindvf;
} efs1(1), efs2(2), efs3(3), efs4(4), efs5(5), efs6(6), efs7(7), efs8(8), efs9(9), efs10(10);

function<void()> EventFunctions::bindvf = bind(&EventFunctions::vf, &efs1);

void eventTest()
{
    printf("\n%d %s\n\n", __LINE__, __FUNCTION__);

    Event<void(), _UseLastValue, int, less<int>> ev;
    printf("test varing insert delegate\n");
    ev.pushBack(EventFunctions::bindvf);
    ev.pushBack(EventFunctions::bindvf, &EventFunctions::bindvf);
    ev.pushBack(EventFunctions::svf);
    ev.pushBack(&EventFunctions::vf, &efs1);
    //ev.pushBack(&VoidFunction::func, make_shared<VoidFunction>(6));

    ev.raise();
    ev.clear();
    printf("\n%d %s\n\n", __LINE__, __FUNCTION__);

    printf("test pushback order(8-1):\n");
    ev.pushBack(&EventFunctions::vf, &efs8);
    ev.pushBack(&EventFunctions::vf, &efs7);
    //ev.pushBack(&EventFunctions::vf, &efs1);

    ev.pushBack(3, &EventFunctions::vf, &efs6);
    ev.pushBack(3, &EventFunctions::vf, &efs5);
    ev.pushBack(2, &EventFunctions::vf, &efs4);
    ev.pushBack(2, &EventFunctions::vf, &efs3);
    ev.pushBack(1, &EventFunctions::vf, &efs2);
    ev.pushBack(1, &EventFunctions::vf, &efs1);

    ev.raise(); // should output 2 1 4 3 6 5 8 7
    ev.clear();

    printf("test pushfront order(1-8):\n");
    ev.pushFront(&EventFunctions::vf, &efs1);
    ev.pushFront(&EventFunctions::vf, &efs2);
    //ev.pushBack(&EventFunctions::vf, &efs1);

    ev.pushFront(1, &EventFunctions::vf, &efs3);
    ev.pushFront(1, &EventFunctions::vf, &efs4);
    ev.pushFront(2, &EventFunctions::vf, &efs5);
    ev.pushFront(2, &EventFunctions::vf, &efs6);
    ev.pushFront(3, &EventFunctions::vf, &efs7);
    ev.pushFront(3, &EventFunctions::vf, &efs8);

    ev.raise(); // shoud output 2 1 4 3 6 5 8 7
    ev.clear();

    printf("test pushback and front(GbUbUb GfUfUf Gb:123 456 7):\n");
    ev.pushBack(0, &EventFunctions::vf, &efs1);
    ev.pushBack(&EventFunctions::vf, &efs2);
    ev.pushBack(&EventFunctions::vf, &efs3);

    ev.pushFront(0, &EventFunctions::vf, &efs4);
    ev.pushFront(&EventFunctions::vf, &efs5);
    ev.pushFront(&EventFunctions::vf, &efs6);

    ev.pushBack(0, &EventFunctions::vf, &efs7);

    ev.raise(); // should output 6 5 4 1 7 2 3
    ev.clear();

    {
        Event<void(), _UseLastValue, int, greater<int>> ev;
        printf("test group compare by greater (1-5):\n");
        ev.pushFront(&EventFunctions::vf, &efs1);
        ev.pushFront(2, &EventFunctions::vf, &efs2);
        ev.pushBack(2, &EventFunctions::vf, &efs3);
        ev.pushBack(1, &EventFunctions::vf, &efs4);
        ev.pushBack(&EventFunctions::vf, &efs5);

        ev.raise(); // should output 1 2 3 4 5
        ev.clear();
    }
    eventProfile();
}

}   // namespace cc
