#include "pch.h"

int perfLog(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int ret = __android_log_vprint(ANDROID_LOG_INFO, "performance", fmt, args);
    va_end(args);
    return ret;
}
