#include "pch.h"

#include "Performace.h"

#include "profile.h"

using namespace std;

#define DO_SOMETHING \
{ \
    static unsigned id; \
    id = ProfileStart(id, __FUNCTION__); \
    ProfileStop(id); \
}

void cStyleFunction()
{
    DO_SOMETHING;
}

class TestVirtualMemberFunctionBase
{
public:
    virtual void virtualMemberFunction();
};
void TestVirtualMemberFunctionBase::virtualMemberFunction()
{
    DO_SOMETHING;
}

class TestVirtualMemberFunction : public TestVirtualMemberFunctionBase
{
public:
    virtual void virtualMemberFunction();
};
void TestVirtualMemberFunction::virtualMemberFunction()
{
    DO_SOMETHING;
}

void profileFunction()
{
    LogInfo("Begin profileFunction");
    static const unsigned maxTimes = 1000000L;
////////////////////////////////////////////////////////////////////////////////
    static unsigned perfCStyleFun;
    perfCStyleFun = ProfileStart(perfCStyleFun, "cStyleFunction");
    for (unsigned i = 0; i < maxTimes; ++i)
    {
        cStyleFunction();
    }
    ProfileStop(perfCStyleFun);

////////////////////////////////////////////////////////////////////////////////
    static unsigned perfVertualMemFun;
    perfVertualMemFun = ProfileStart(perfVertualMemFun, "VertualMemFunction");
    TestVirtualMemberFunction test;
    TestVirtualMemberFunctionBase * pTestBase = &test;
    for (unsigned i = 0; i < maxTimes; ++i)
    {
        pTestBase->virtualMemberFunction();
    }
    ProfileStop(perfVertualMemFun);

////////////////////////////////////////////////////////////////////////////////
    static unsigned perfStdFunction;
    perfStdFunction = ProfileStart(perfStdFunction, "StdFunction");
    function<void()> stdFunction = cStyleFunction;
    for (unsigned i = 0; i < maxTimes; ++i)
    {
        stdFunction();
    }
    ProfileStop(perfStdFunction);

////////////////////////////////////////////////////////////////////////////////
    static unsigned perfStdFunctionBind;
    perfStdFunctionBind = ProfileStart(perfStdFunctionBind, "StdFunctionBind");
    function<void()> stdFunctionBind = 
        bind(&TestVirtualMemberFunction::virtualMemberFunction, &test);
    for (unsigned i = 0; i < maxTimes; ++i)
    {
        stdFunctionBind();
    }
    ProfileStop(perfStdFunctionBind);

////////////////////////////////////////////////////////////////////////////////
    static unsigned perfMemFn;
    perfMemFn = ProfileStart(perfMemFn, "StdMemFn");
    auto stdMemFn = 
        mem_fn(&TestVirtualMemberFunction::virtualMemberFunction);
    for (unsigned i = 0; i < maxTimes; ++i)
    {
        stdMemFn(&test);
    }
    ProfileStop(perfMemFn);

    LogInfo("End profileFunction.");
}
