#ifndef __FUNCATION__FUNDATION_MACROS_CPP_H__
#define __FUNCATION__FUNDATION_MACROS_CPP_H__

extern int printf_stdout_logfile(const char *format, ...);
#define printf  printf_stdout_logfile

#include <stdio.h>
#define CCLOG   printf

#define CCFUNCTION_SIGN  __FUNCSIG__

// Need define IMPLEMENT_CLASS name first
#define IMPLEMENT_FUNCTION(name, ...) \
    IMPLEMENT_CLASS##::name(__VA_ARGS__) \
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
        CCLOG("Enter %s\n", _funcSign);
    }
    ~_FuncGuardClass()
    {
        CCLOG("Exist %s\n", _funcSign);
    }
private:
    const char * _funcSign;
};

#endif  // __FUNCATION__FUNDATION_MACROS_CPP_H__
