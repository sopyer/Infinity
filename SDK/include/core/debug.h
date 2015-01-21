#pragma once

#ifdef __cplusplus
extern "C" {
#endif
int assert_handler(const char* cond, const char* file, int line);
#ifdef __cplusplus
}
#endif

#if defined(_MSC_VER)
    extern void __cdecl __debugbreak(void);
    #define CORE_BREAK() __debugbreak()
#elif (defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__)))
    #define CORE_BREAK() __asm__ __volatile__ ( "int $3\n\t" )
#elif defined(HAVE_SIGNAL_H)
    #include <signal.h>
    #define CORE_BREAK() raise(SIGTRAP)
#else
    #error "Implement breakpoint!"
#endif

#ifdef CORE_ENABLE_ASSERT
	#define assert(x) ((void)(!(x) && assert_handler(#x, __FILE__, __LINE__) && (CORE_BREAK(), 1)))
#else
	#define assert(x) ((void)sizeof(x))
#endif
