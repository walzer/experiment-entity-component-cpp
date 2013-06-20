#include "pch.h"

#include "Event.h"

#define REPEAT for (int i = 0; i < 100; ++i)

namespace cc {;

using namespace std;

template <
    typename Signature,
    typename Combiner = _UseLastValue
>
class TestEvent {
public:
    typedef typename function<Signature>::result_type ResultType;

    ResultType raiseWithoutCheck(const Event<Signature> &e) {
        for_each(e._delegates.begin(), e._delegates.end(), [](const Event<Signature>::DelegateType &delegate) {
            delegate.function();
        });
        return ResultType();
    }
    ResultType raiseWithCheck(Event<Signature> &e) {
        for_each(e._delegates.begin(), e._delegates.end(), [](const Event<Signature>::DelegateType &delegate) {
            if (delegate.disabled()) {
                return;
            }
            delegate.function();
        });
        remove_if(e._delegates.begin(), e._delegates.end(), [&e](const Event<Signature>::DelegateType &delegate) {
            if (delegate.disabled()) {
                e._address.erase(delegate.getAddress());
                return true;
            }
            return false;
        });
        return ResultType();
    }
    ResultType raiseWithCheck2(Event<Signature> &e) {
        auto end = e._delegates.end();
        for (auto iter = e._delegates.begin(); iter != end; ) {
            typename Event<Signature>::DelegateType &delegate=*iter;
            if (delegate.disabled()) {
                e._address.erase(iter->getAddress());
                e._delegates.erase(iter++);
                continue;
            }
            delegate.function();
            ++iter;
        }
    }
};

void eventProfile() {
    printf("\n%d %s\n\n", __LINE__, __FUNCTION__);

    static const int MAX_NUM = 10000;
    unique_ptr<char[]> address(new char[MAX_NUM]);

    Event<void()> ve;
    TestEvent<void()> tve;
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
    REPEAT{
        ProfileFunction(tve.raiseWithoutCheck(ve), "tve::raiseWithoutCheck");
    }
    REPEAT{
        ProfileFunction(tve.raiseWithCheck(ve), "tve::raiseWithCheck");
    }
    REPEAT{
        ProfileFunction(tve.raiseWithCheck2(ve), "tve::raiseWithCheck2");
    }

    ve.clear();
    ////////////////////////////////////////////////////////////////////////////
    Event<int()> ie;
    TestEvent<int()> tie;
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
    REPEAT{
        ProfileFunction(reti = tie.raiseWithoutCheck(ie), "tie::raiseWithoutCheck");
    }
    cout << "tie::raiseWithoutCheck() return: " << reti << endl;
    REPEAT{
        ProfileFunction(reti = tie.raiseWithCheck(ie), "tie::raiseWithCheck");
    }
    cout << "tie::raiseWithCheck() return: " << reti << endl;
    REPEAT{
        ProfileFunction(reti = tie.raiseWithCheck2(ie), "tie::raiseWithCheck2");
    }
    cout << "tie::raiseWithCheck2() return: " << reti << endl;

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
} efs(1);

function<void()> EventFunctions::bindvf = bind(&EventFunctions::vf, &efs);

void eventTest()
{
    printf("\n%d %s\n\n", __LINE__, __FUNCTION__);

    Event<void()> ev;
    printf("test forward call\n");
    ev.pushBack(EventFunctions::bindvf);
    ev.pushBack(EventFunctions::bindvf, &EventFunctions::bindvf);
    ev.pushBack(EventFunctions::svf);
    ev.pushBack(&EventFunctions::vf, &efs);
    //ev.pushBack(&VoidFunction::func, make_shared<VoidFunction>(6));

    ev.raise();
    printf("\n%d %s\n\n", __LINE__, __FUNCTION__);
    eventProfile();
}

}   // namespace cc
