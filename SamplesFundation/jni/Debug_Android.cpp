#include "Debug.h"

#include <stdarg.h>

#include <android/log.h>

void Debug::logd(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_DEBUG, "samples", fmt, args);
    va_end(args);
}

void Debug::loge(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_ERROR, "samples", fmt, args);
    va_end(args);
}
