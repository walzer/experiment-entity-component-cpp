#ifndef __FOUNDATION__CCFOUNDATION_MACROS_H_H__
#define __FOUNDATION__CCFOUNDATION_MACROS_H_H__

#if defined(WIN32)
#include <stdio.h>
#define CCLOGI printf
#define CCLOGW printf
#endif  // WIN32

#define CCEXPEND(x) \
    x
#define CCTO_STRING(t) \
    #t

// Define CCGLUE macro.
#define _CCGLUE_IMPL(a, b) \
    a##b
#define _CCGLUE_EXPEND(a, b) \
    _CCGLUE_IMPL(a, b)
#define CCGLUE(a, b) \
    _CCGLUE_EXPEND(a, b)
// Define CCGLUE macro.

// Define CCAPPEND_NARGS macro.
#define _CCNARGS_IMPL( \
    _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
    _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
    N,...) N
#define CCNARGS(...) \
    CCEXPEND(_CCNARGS_IMPL(__VA_ARGS__, \
    20,19,18,17,16,15,14,13,12,11, \
    10,9,8,7,6,5,4,3,2,1,0))
#define CCAPPEND_NARGS(name, ...) \
    CCGLUE(name, CCNARGS(__VA_ARGS__))
// Define CCAPPEND_NARGS macro.

// Define CCFOR_EACH macro.
#define _APPLY_MACRO_1(m, a1) \
    m(a1)
#define _APPLY_MACRO_2(m, a1, a2) \
    m(a1), m(a2)
#define _APPLY_MACRO_3(m, a1, a2, a3) \
    m(a1), _APPLY_MACRO_2(m, a2, a3)
#define _APPLY_MACRO_4(m, a1, a2, a3, a4) \
    m(a1), _APPLY_MACRO_3(m, a2, a3, a4)
#define _APPLY_MACRO_5(m, a1, a2, a3, a4, a5) \
    m(a1), _APPLY_MACRO_4(m, a2, a3, a4, a5)
#define _APPLY_MACRO_6(m, a1, a2, a3, a4, a5, a6) \
    m(a1), _APPLY_MACRO_5(m, a2, a3, a4, a5, a6)
#define _APPLY_MACRO_7(m, a1, a2, a3, a4, a5, a6, a7) \
    m(a1), _APPLY_MACRO_6(m, a2, a3, a4, a5, a6, a7)
#define _APPLY_MACRO_8(m, a1, a2, a3, a4, a5, a6, a7, a8) \
    m(a1), _APPLY_MACRO_7(m, a2, a3, a4, a5, a6, a7, a8)
#define _APPLY_MACRO_9(m, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
    m(a1), _APPLY_MACRO_8(m, a2, a3, a4, a5, a6, a7, a8, a9)
#define CCFOR_EACH(macro, ...) \
    CCAPPEND_NARGS(_APPLY_MACRO_, __VA_ARGS__)CCEXPEND((macro, __VA_ARGS__))
// Define CCFOR_EACH macro.

// Define CCFOR_EACH_NUM macro.
#define _APPLY_MACRO_NUM_1(m, a1) \
    m(1, a1)
#define _APPLY_MACRO_NUM_2(m, a1, a2) \
    _APPLY_MACRO_NUM_1(m, a1), m(2, a2)
#define _APPLY_MACRO_NUM_3(m, a1, a2, a3) \
    _APPLY_MACRO_NUM_2(m, a1, a2), m(3, a3)
#define _APPLY_MACRO_NUM_4(m, a1, a2, a3, a4) \
    _APPLY_MACRO_NUM_3(m, a1, a2, a3), m(4, a4)
#define _APPLY_MACRO_NUM_5(m, a1, a2, a3, a4, a5) \
    _APPLY_MACRO_NUM_4(m, a1, a2, a3, a4), m(5, a5)
#define _APPLY_MACRO_NUM_6(m, a1, a2, a3, a4, a5, a6) \
    _APPLY_MACRO_NUM_5(m, a1, a2, a3, a4, a5), m(6, a6)
#define _APPLY_MACRO_NUM_7(m, a1, a2, a3, a4, a5, a6, a7) \
    _APPLY_MACRO_NUM_6(m, a1, a2, a3, a4, a5, a6), m(7, a7)
#define _APPLY_MACRO_NUM_8(m, a1, a2, a3, a4, a5, a6, a7, a8) \
    _APPLY_MACRO_NUM_7(m, a1, a2, a3, a4, a5, a6, a7), m(8, a8)
#define _APPLY_MACRO_NUM_9(m, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
    _APPLY_MACRO_NUM_8(m, a1, a2, a3, a4, a5, a6, a7, a8), m(9, a9)
#define CCFOR_EACH_NUM(macro, ...) \
    CCAPPEND_NARGS(_APPLY_MACRO_NUM_, __VA_ARGS__)CCEXPEND((macro, __VA_ARGS__))
// Define CCFOR_EACH_NUM macro.

// Define CCTYPES_APPEND_PARAS, CCTYPES_TO_PARAS, CCTYPES_WITH_TYPENAME macro.
#define _TYPE_APPEND_PARA(n, type) type CCGLUE(para, n)
#define _TYPE_TO_PARA(n, type) CCGLUE(para, n)
#define _TYPE_WITH_TYPENAME(type) typename type
#define CCTYPES_APPEND_PARAS(...) \
    CCFOR_EACH_NUM(_TYPE_APPEND_PARA, __VA_ARGS__)
#define CCTYPES_TO_PARAS(...) \
    CCFOR_EACH_NUM(_TYPE_TO_PARA, __VA_ARGS__)
#define CCTYPES_WITH_TYPENAME(...) \
    CCFOR_EACH(_TYPE_WITH_TYPENAME, __VA_ARGS__)
// Define CCTYPES_APPEND_PARAS, CCTYPES_TO_PARAS, CCTYPES_WITH_TYPENAME macro.

#endif  // __FOUNDATION__CCFOUNDATION_MACROS_H_H__
