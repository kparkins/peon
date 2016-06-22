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
    #define PEON_FORCE_INLINE inline __forceinline
    #define PEON_ALIGN(bytes) __declspec(align(bytes))
#elif defined(__GNUC__)
    #define PEON_FORCE_INLINE inline __attribute__ ((always_inline))
    #define PEON_ALIGN(bytes) __attribute__ ((aligned(bytes)))
#endif


#define PEON_CONCAT(A, B) A ## B
#define PEON_MANGLE(NAME, NUM) PEON_CONCAT(NAME ## _, NUM)

#define PEON_GET_COUNT(_1, _2, _3, _4, _5, TOTAL, ...) TOTAL
#define PEON_NUM_ARGS(...) PEON_GET_COUNT(__VA_ARGS__, 5, 4, 3, 2, 1)

#define PEON_SELECT_MACRO(MACRO_NAME, ...) PEON_MANGLE(MACRO_NAME, PEON_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)


#define PEON_DONT_CARE GLFW_DONT_CARE
#define PEON_OPENGL_ANY_PROFILE GLFW_OPENGL_ANY_PROFILE;
#define PEON_OPENGL_COMPAT_PROFILE GLFW_OPENGL_COMPAT_PROFILE;
#define PEON_OPENGL_CORE_PROFILE GLFW_OPENGL_CORE_PROFILE;

#endif /* PEON_MACROS_H */
