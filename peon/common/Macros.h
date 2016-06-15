/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#ifndef PEON_MACROS_H
#define PEON_MACROS_H

#if defined (__APPLE__) || defined(__MACH__)
    #define __FILENAME__ (strrchr(__FILE__, '/') ? (char*) strrchr(__FILE__, '/') + 1 : __FILE__)
#elif defined(_WIN32)
    #define __FILENAME__ (strrchr(__FILE__, '\\') ? (char*) strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif

#if defined(_MSC_VER)
    #define FORCE_INLINE inline __forceinline
    #define ALIGN_16 __declspec(align(16))
#elif defined(__GNUC__)
    #define FORCE_INLINE inline __attribute__ ((always_inline))
    #define ALIGN_16 __attribute__ ((aligned(16)))
#endif


#define CONCAT(A, B) A ## B
#define MANGLE(NAME, NUM) CONCAT(NAME ## _, NUM)

#define GET_COUNT(_1, _2, _3, _4, _5, TOTAL, ...) TOTAL
#define NUM_ARGS(...) GET_COUNT(__VA_ARGS__, 5, 4, 3, 2, 1)

#define SELECT_MACRO(MACRO_NAME, ...) MANGLE(MACRO_NAME, NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)


#endif /* PEON_MACROS_H */
