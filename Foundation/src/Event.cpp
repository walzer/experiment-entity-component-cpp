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

void eventTest()
{
    eventProfile();
}

}   // namespace cc
