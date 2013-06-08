#ifndef __FOUNDATION__FOUNDATION_MACROS_CPP_H__
#define __FOUNDATION__FOUNDATION_MACROS_CPP_H__

#include "FundationMacrosH.h"

/*
 *  Doesn't include this file in header file.
 */

#include "FundationMacrosH.h"

#define CC_IMPL_FUNCTION_GUARD \
    ::cc::_FuncGuardClass _functionGuard(__FUNCTION__);

namespace cc {;

class _FuncGuardClass
{
public:
    _FuncGuardClass(const char * funcSign)
    {
        _funcSign = funcSign;
        CC_LOGI("Enter %s\n", _funcSign);
    }
    ~_FuncGuardClass()
    {
        CC_LOGI("Exist %s\n", _funcSign);
    }
private:
    const char * _funcSign;
};

}   // namespace cc

#endif  // __FOUNDATION__FOUNDATION_MACROS_CPP_H__
