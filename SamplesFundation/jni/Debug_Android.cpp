#include "Debug.h"

#include <stdarg.h>

#include <android/log.h>

void Debug::logd(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __android_log_print(ANDROID_LOG_DEBUG, "samples", fmt, args);
    va_end(args);
}
