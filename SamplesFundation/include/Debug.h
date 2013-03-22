#ifndef DEBUG_H_
#define DEBUG_H_


struct Debug
{
    static void logd(const char * fmt, ...);
    static void loge(const char * fmt, ...);
};

#endif /* DEBUG_H_ */
