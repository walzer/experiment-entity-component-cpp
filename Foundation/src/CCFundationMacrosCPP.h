#ifndef __FOUNDATION__FOUNDATION_MACROS_CPP_H__
#define __FOUNDATION__FOUNDATION_MACROS_CPP_H__

#include "CCFundationMacrosH.h"


#define CCFUNCTION_SIGN  __FUNCSIG__

// Need define IMPLEMENT_CLASS name first
#define IMPLEMENT_FUNCTION(name, ...) \
    IMPLEMENT_CLASS :: name(__VA_ARGS__) \
{ \
    _IMPLEMENT_FUNCTION_GUARD;

#define _IMPLEMENT_FUNCTION_GUARD \
    _FuncGuardClass(CCFUNCTION_SIGN);

class _FuncGuardClass
{
public:
    _FuncGuardClass(const char * funcSign)
    {
        _funcSign = funcSign;
        CCLOGI("Enter %s\n", _funcSign);
    }
    ~_FuncGuardClass()
    {
        CCLOGI("Exist %s\n", _funcSign);
    }
private:
    const char * _funcSign;
};

#endif  // __FOUNDATION__FOUNDATION_MACROS_CPP_H__
