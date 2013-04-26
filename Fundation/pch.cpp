#include "pch.h"

#define CCNARGS(...) \
    _CCNARGS_EXPAND(_CCNARGS_IMPL(__VA_ARGS__, \
    20,19,18,17,16,15,14,13,12,11, \
    10,9,8,7,6,5,4,3,2,1,0))

#define _CCNARGS_EXPAND(x)    x
#define _CCNARGS_IMPL( \
    _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
    _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
    N,...) N

#define CCGLUE(a, b) \
    _CCGLUE_EXPEND(a, b)

#define _CCGLUE_EXPEND(a, b) _CCGLUE_IMPL(a, b)
#define _CCGLUE_IMPL(a, b)  a##b

#define CCEXPAND_NARGS(name, ...) \
    CCGLUE(name, CCNARGS(__VA_ARGS__))

int i = CCNARGS(a, b, c);

class A
{
    void func();
    void func(int a);
    void func(int a, int b);
};

#define IMPLEMENT_CLASS A
void IMPLEMENT_FUNCTION(func)

}

void IMPLEMENT_FUNCTION(func, int a)

}

void IMPLEMENT_FUNCTION(func, int a, int b)

}