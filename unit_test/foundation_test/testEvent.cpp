#include "pch.h"

#include "Event.h"

using namespace cc;

/* key: function name, value: object id */
static vector<pair<const char *, int>> calledFunctions;

#define MEM_FN(ret, name) \
    ret name() { calledFunctions.push_back(make_pair(#name, id)); return ret(); } \
    ret name##i(int i1) { calledFunctions.push_back(make_pair(#name"i", id)); return ret(); } \
    ret name##ii(int i1, int i2) { calledFunctions.push_back(make_pair(#name"ii", id)); return ret(); } \
    ret name##iii(int i1, int i2, int i3) { calledFunctions.push_back(make_pair(#name"iii", id)); return ret(); } \
    ret name##iiii(int i1, int i2, int i3, int i4) { calledFunctions.push_back(make_pair(#name"iiii", id)); return ret(); }

#define STATIC_FN(ret, name) \
    static ret name() { calledFunctions.push_back(make_pair(#name, -1)); return ret(); } \
    static ret name##i(int i1) { calledFunctions.push_back(make_pair(#name"i", -1)); return ret(); } \
    static ret name##ii(int i1, int i2) { calledFunctions.push_back(make_pair(#name"ii", -1)); return ret(); } \
    static ret name##iii(int i1, int i2, int i3) { calledFunctions.push_back(make_pair(#name"iii", -1)); return ret(); } \
    static ret name##iiii(int i1, int i2, int i3, int i4) { calledFunctions.push_back(make_pair(#name"iiii", -1)); return ret(); }

struct EventFunctions {
    static int s_serial;

    EventFunctions() : id(++s_serial){
    }
    int id;

    MEM_FN(void, fnv)
    STATIC_FN(void, sfnv)

    MEM_FN(int, fni)
    STATIC_FN(int, sfni)

    // bind functions
    static void bindfnv() { calledFunctions.push_back(make_pair("bindfnv", -1)); }
    static function<void()> bfnv;
};
int EventFunctions::s_serial = 0;
function<void()> EventFunctions::bfnv = bind(&EventFunctions::bindfnv);

class EventTest : public testing::Test {
public:
    static void SetUpTestCase() {
        cout << "\nEventTest SetUpTestCase before run it's tests\n" << endl;
    }

    static void TearDownTestCase() {
        cout << "\nnEventTest TearDownTestCase after it's tests runed\n" << endl;
    }

    virtual void SetUp() {
        cout << "test SetUp" << endl;
        evUntil = [](){return true;};
        eiUntil = [](int){return true;};
    }

    virtual void TearDown() {
        EventFunctions::s_serial = 0;
        cout << "test TearDown" << endl;
    }

    /*
        AT_FRONT: 0, 1
        Group 1 : 2, 3
        Group 3 : 4, 5
        Group 5 : 6, 7
        AT_BACK : 8, 9
    */
    EventFunctions objs[10];
    function<bool()> evUntil;   // event<void> until prediction
    function<bool(int)> eiUntil;
};

TEST_F(EventTest, pushAndRemove) {
    cout << "Use Event<void()> type for :\n";
    Event<void()> e;
    decltype(calledFunctions.begin()) iter;

    cout << "testing function object\n";
    auto handle = e.push(EventFunctions::bfnv);
    e.raise();
    ASSERT_EQ(1, calledFunctions.size());
    iter = calledFunctions.begin();
    EXPECT_STREQ("bindfnv", iter->first);
    calledFunctions.clear();

    handle.disable();
    e.raise();
    ASSERT_EQ(0, calledFunctions.size());
    calledFunctions.clear();

    cout << "testing function object with address\n";
    e.push(EventFunctions::bfnv, &EventFunctions::bfnv);
    e.raise();
    ASSERT_EQ(1, calledFunctions.size());
    iter = calledFunctions.begin();
    EXPECT_STREQ("bindfnv", iter->first);
    calledFunctions.clear();

    e.remove(&EventFunctions::bfnv);
    e.raise();
    ASSERT_EQ(0, calledFunctions.size());
    calledFunctions.clear();

    cout << "testing C-style function\n";
    e.push(&EventFunctions::sfnv);
    e.raise();
    ASSERT_EQ(1, calledFunctions.size());
    iter = calledFunctions.begin();
    EXPECT_STREQ("sfnv", iter->first);
    calledFunctions.clear();

    e.remove(&EventFunctions::sfnv);
    e.raise();
    ASSERT_EQ(0, calledFunctions.size());
    calledFunctions.clear();

    cout << "testing member function\n";
    e.push(&EventFunctions::fnv, &objs[0]);
    e.raise();
    ASSERT_EQ(1, calledFunctions.size());
    iter = calledFunctions.begin();
    EXPECT_STREQ("fnv", iter->first);
    calledFunctions.clear();

    e.remove(&objs[0]);
    e.raise();
    ASSERT_EQ(0, calledFunctions.size());
    calledFunctions.clear();

    cout << "testing member function track shared object\n";
    auto obj = make_shared<EventFunctions>();
    e.push(&EventFunctions::fnv, obj.get()).track(obj);
    e.raise();
    ASSERT_EQ(1, calledFunctions.size());
    iter = calledFunctions.begin();
    EXPECT_STREQ("fnv", iter->first);
    calledFunctions.clear();

    obj.reset();
    e.raise();
    ASSERT_EQ(0, calledFunctions.size());
    calledFunctions.clear();
}

struct AtBack {};
struct AtFront {};

template <typename T>
void PushDelegateToEvent(Event<void()> &e, EventFunctions *objs);

template <>
void PushDelegateToEvent<AtBack>(
    Event<void()> &e,
    EventFunctions *objs
) {
    e.push(&EventFunctions::fnv, &objs[1], DelegatePosition::AT_FRONT);
    e.push(&EventFunctions::fnv, &objs[0], DelegatePosition::AT_FRONT);
    e.push(1, &EventFunctions::fnv, &objs[2]);
    e.push(1, &EventFunctions::fnv, &objs[3]);
    e.push(3, &EventFunctions::fnv, &objs[4]);
    e.push(3, &EventFunctions::fnv, &objs[5]);
    e.push(5, &EventFunctions::fnv, &objs[6]);
    e.push(5, &EventFunctions::fnv, &objs[7]);
    e.push(&EventFunctions::fnv, &objs[8]);
    e.push(&EventFunctions::fnv, &objs[9]);
}

template <>
void PushDelegateToEvent<AtFront>(
    Event<void()> &e,
    EventFunctions *objs
) {
    e.push(&EventFunctions::fnv, &objs[1], DelegatePosition::AT_FRONT);
    e.push(&EventFunctions::fnv, &objs[0], DelegatePosition::AT_FRONT);
    e.push(1, &EventFunctions::fnv, &objs[3], DelegatePosition::AT_FRONT);
    e.push(1, &EventFunctions::fnv, &objs[2], DelegatePosition::AT_FRONT);
    e.push(3, &EventFunctions::fnv, &objs[5], DelegatePosition::AT_FRONT);
    e.push(3, &EventFunctions::fnv, &objs[4], DelegatePosition::AT_FRONT);
    e.push(5, &EventFunctions::fnv, &objs[7], DelegatePosition::AT_FRONT);
    e.push(5, &EventFunctions::fnv, &objs[6], DelegatePosition::AT_FRONT);
    e.push(&EventFunctions::fnv, &objs[8]);
    e.push(&EventFunctions::fnv, &objs[9]);
}

template <typename PushPosition>
class EventGroupTest : public EventTest {
    typedef EventTest BaseType;
public:
    virtual void SetUp() {
        BaseType::SetUp();
        PushDelegateToEvent<PushPosition>(e, objs);
    }

    Event<void()> e;
};

typedef testing::Types<AtBack, AtFront> PushDelegateAtPositions;
TYPED_TEST_CASE(EventGroupTest, PushDelegateAtPositions);

TYPED_TEST(EventGroupTest, raiseAndRemove) {
    e.raise();
    ASSERT_EQ(10, calledFunctions.size());
    for (unsigned i = 0; i < calledFunctions.size(); ++i) {
        EXPECT_EQ(i + 1, calledFunctions[i].second);
    }
    calledFunctions.clear();

    e.raiseUntil(evUntil);
    ASSERT_EQ(1, calledFunctions.size());
    EXPECT_EQ(1, calledFunctions[0].second);
    calledFunctions.clear();

    e.remove(DelegatePosition::AT_BACK);
    e.raise();
    ASSERT_EQ(8, calledFunctions.size());
    for (unsigned i = 0; i < calledFunctions.size(); ++i) {
        EXPECT_EQ(i + 1, calledFunctions[i].second);
    }
    calledFunctions.clear();

    e.remove(DelegatePosition::AT_FRONT);
    e.raise();
    ASSERT_EQ(6, calledFunctions.size());
    for (unsigned i = 0; i < calledFunctions.size(); ++i) {
        EXPECT_EQ(i + 3, calledFunctions[i].second);
    }
    calledFunctions.clear();

    e.remove(3);
    e.raise();
    ASSERT_EQ(4, calledFunctions.size());
    EXPECT_EQ(3, calledFunctions[0].second);
    EXPECT_EQ(4, calledFunctions[1].second);
    EXPECT_EQ(7, calledFunctions[2].second);
    EXPECT_EQ(8, calledFunctions[3].second);
    calledFunctions.clear();
}

TYPED_TEST(EventGroupTest, groupRaiseAndRemove) {
    e.raiseGroup(1);
    ASSERT_EQ(2, calledFunctions.size());
    for (unsigned i = 0; i < calledFunctions.size(); ++i) {
        EXPECT_EQ(i + 3, calledFunctions[i].second);
    }
    calledFunctions.clear();

    e.raiseGroupUntil(1, evUntil);
    ASSERT_EQ(1, calledFunctions.size());
    EXPECT_EQ(3, calledFunctions[0].second);
    calledFunctions.clear();

    e.raiseGroup(3);
    ASSERT_EQ(2, calledFunctions.size());
    for (unsigned i = 0; i < calledFunctions.size(); ++i) {
        EXPECT_EQ(i + 5, calledFunctions[i].second);
    }
    calledFunctions.clear();

    e.raiseGroupUntil(3, evUntil);
    ASSERT_EQ(1, calledFunctions.size());
    EXPECT_EQ(5, calledFunctions[0].second);
    calledFunctions.clear();

    e.raiseGroup(5);
    ASSERT_EQ(2, calledFunctions.size());
    for (unsigned i = 0; i < calledFunctions.size(); ++i) {
        EXPECT_EQ(i + 7, calledFunctions[i].second);
    }
    calledFunctions.clear();

    e.raiseGroupUntil(5, evUntil);
    ASSERT_EQ(1, calledFunctions.size());
    EXPECT_EQ(7, calledFunctions[0].second);
    calledFunctions.clear();
}

TEST_F(EventTest, withReturnValue) {
    Event<int()> e;
    auto handle = e.push(
        bind(&EventFunctions::fni, &objs[1]),
        DelegatePosition::AT_FRONT
    );
    function<int()> func = bind(&EventFunctions::fni, &objs[0]);
    e.push(func, &func, DelegatePosition::AT_FRONT);
    e.push(1, &EventFunctions::fni, &objs[2]);
    e.push(1, &EventFunctions::fni, &objs[3]);
    e.push(3, &EventFunctions::fni, &objs[5], DelegatePosition::AT_FRONT);
    e.push(3, &EventFunctions::fni, &objs[4], DelegatePosition::AT_FRONT);
    e.push(&EventFunctions::sfni);
    e.raise();
    ASSERT_EQ(7, calledFunctions.size());
    for (unsigned i = 0; i < calledFunctions.size() - 1; ++i) {
        EXPECT_EQ(i + 1, calledFunctions[i].second);
    }
    EXPECT_EQ(-1, calledFunctions[calledFunctions.size() - 1].second);
    calledFunctions.clear();

    e.raiseGroup(1);
    ASSERT_EQ(2, calledFunctions.size());
    for (unsigned i = 0; i < calledFunctions.size() - 1; ++i) {
        EXPECT_EQ(i + 3, calledFunctions[i].second);
    }
    calledFunctions.clear();

    handle.disable();
    e.remove(&func);
    e.remove(&objs[2]);
    e.remove(&objs[3]);
    e.remove(3);
    e.remove(&EventFunctions::sfni);
    e.raise();
    ASSERT_EQ(0, calledFunctions.size());
}

TEST_F(EventTest, withArgs) {
    Event<void(int, int, int, int)> e;
    auto handle = e.push(
        bind(
            &EventFunctions::fnviiii,
            &objs[1],
            placeholders::_1,
            placeholders::_2,
            placeholders::_3,
            placeholders::_4
        ),
        DelegatePosition::AT_FRONT
    );
    function<void(int, int, int, int)> func = bind(
        &EventFunctions::fnviiii,
        &objs[0],
        placeholders::_1,
        placeholders::_2,
        placeholders::_3,
        placeholders::_4
    );
    e.push(func, &func, DelegatePosition::AT_FRONT);
    e.push(1, &EventFunctions::fnviiii, &objs[2]);
    e.push(1, &EventFunctions::fnviiii, &objs[3]);
    e.push(3, &EventFunctions::fnviiii, &objs[5], DelegatePosition::AT_FRONT);
    e.push(3, &EventFunctions::fnviiii, &objs[4], DelegatePosition::AT_FRONT);
    e.push(&EventFunctions::sfnviiii);
    e.raise(1, 2, 3, 4);
    ASSERT_EQ(7, calledFunctions.size());
    for (unsigned i = 0; i < calledFunctions.size() - 1; ++i) {
        EXPECT_EQ(i + 1, calledFunctions[i].second);
    }
    EXPECT_EQ(-1, calledFunctions[calledFunctions.size() - 1].second);
    calledFunctions.clear();

    e.raiseGroup(1, 1, 2, 3, 4);
    ASSERT_EQ(2, calledFunctions.size());
    for (unsigned i = 0; i < calledFunctions.size() - 1; ++i) {
        EXPECT_EQ(i + 3, calledFunctions[i].second);
    }
    calledFunctions.clear();

    handle.disable();
    e.remove(&func);
    e.remove(&objs[2]);
    e.remove(&objs[3]);
    e.remove(3);
    e.remove(&EventFunctions::sfnviiii);
    e.raise(1, 2, 3, 4);
    ASSERT_EQ(0, calledFunctions.size());
}

TEST_F(EventTest, withResultAndArgs) {
    Event<int(int, int, int, int)> e;
    auto handle = e.push(
        bind(
            &EventFunctions::fniiiii,
            &objs[1],
            placeholders::_1,
            placeholders::_2,
            placeholders::_3,
            placeholders::_4
        ),
        DelegatePosition::AT_FRONT
    );
    function<int(int, int, int, int)> func = bind(
        &EventFunctions::fniiiii,
        &objs[0],
        placeholders::_1,
        placeholders::_2,
        placeholders::_3,
        placeholders::_4
    );
    e.push(func, &func, DelegatePosition::AT_FRONT);
    e.push(1, &EventFunctions::fniiiii, &objs[2]);
    e.push(1, &EventFunctions::fniiiii, &objs[3]);
    e.push(3, &EventFunctions::fniiiii, &objs[5], DelegatePosition::AT_FRONT);
    e.push(3, &EventFunctions::fniiiii, &objs[4], DelegatePosition::AT_FRONT);
    e.push(&EventFunctions::sfniiiii);
    e.raise(1, 2, 3, 4);
    ASSERT_EQ(7, calledFunctions.size());
    for (unsigned i = 0; i < calledFunctions.size() - 1; ++i) {
        EXPECT_EQ(i + 1, calledFunctions[i].second);
    }
    EXPECT_EQ(-1, calledFunctions[calledFunctions.size() - 1].second);
    calledFunctions.clear();

    e.raiseGroup(1, 1, 2, 3, 4);
    ASSERT_EQ(2, calledFunctions.size());
    for (unsigned i = 0; i < calledFunctions.size() - 1; ++i) {
        EXPECT_EQ(i + 3, calledFunctions[i].second);
    }
    calledFunctions.clear();

    handle.disable();
    e.remove(&func);
    e.remove(&objs[2]);
    e.remove(&objs[3]);
    e.remove(3);
    e.remove(&EventFunctions::sfniiiii);
    e.raise(1, 2, 3, 4);
    ASSERT_EQ(0, calledFunctions.size());
}
