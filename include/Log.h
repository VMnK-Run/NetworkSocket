#ifndef __LOGg_H__
#define __LOGg_H__
#include <stdio.h>
#include <sys/time.h>

/* Define log level
 * 0: None
 * 1: Error, output error log (red) and variable log (white)
 * 2: Warning, output both error (red), variable (white), and warning log (yellow)
 * 3: Message, output all kinds of log for debug
 */
#define LOG_LEVEL 3


/**
 * 
 * Usage:
 * >>> ERROR("hello world!");
 * >>> WARNING("hello %s", "world!");
 * >>> MSG("this is a message");
 * >>> VAR("speed", "%d %d", 16, 16);
 * >>> TRACE(
 * >>>    int sum = 0;
 * >>>    for (int i = 0; i < 100; i++)
 * >>>        sum += i;
 * >>>    , "A simple for loop"
 *     )
 * 
*/
#define ERROR(format, ...)         __LOG_ERROR__(format, ##__VA_ARGS__)
#define WARNING(format, ...)       __LOG_WARNING__(format, ##__VA_ARGS__)
#define MSG(format, ...)           __LOG_MSG__(format, ##__VA_ARGS__)
#define VAR(var_name, format, ...) __LOG_VAR__(var_name, format, ##__VA_ARGS__)
#define TRACE(code, tag)           __LOG_TRACE__(code, tag)

#define __WHERE__                  __FILE__ ": " "%d"
#define __OUTPUT__(format, ...)    fprintf(stdout, format, ##__VA_ARGS__)

/* log levels */
#define __NONE__    0
#define __ERROR__   1
#define __WARNING__ 2
#define __MSG__     3

/* output string format
 * [file] [line] [color] [log type] [origin format string] [\n+]
 */
#define __AS_ERROR__(format) \
        __WHERE__ "\033[1;31m" "[Error]  " "\033[0m: " format "\n"

#define __AS_WARNING__(format) \
        __WHERE__ "\033[1;33m" "[Warning]" "\033[0m: " format "\n"

#define __AS_MSG__(format) \
        __WHERE__ "\033[1;34m" "[Message]" "\033[0m: " format "\n"

#define __AS_VAR__(var_name, format) \
        __WHERE__ "\033[1;37m" "[Var: " var_name "]" "\033[0m: " format "\n"

/* error */
#if LOG_LEVEL >= __ERROR__
    #define __LOG_ERROR__(format, ...) \
            __OUTPUT__(__AS_ERROR__(format), __LINE__, ##__VA_ARGS__)
#else
    #define __LOG_ERROR__(format, ...) ((void)0)
#endif

/* warning */
#if LOG_LEVEL >= __WARNING__
    #define __LOG_WARNING__(format, ...) \
            __OUTPUT__(__AS_WARNING__(format), __LINE__, ##__VA_ARGS__)
#else
    #define __LOG_WARNING__(format, ...) ((void)0)
#endif

/* message */
#if LOG_LEVEL >= __MSG__
    #define __LOG_MSG__(format, ...) \
            __OUTPUT__(__AS_MSG__(format), __LINE__, ##__VA_ARGS__)
#else
    #define __LOG_MSG__(format, ...) ((void)0)
#endif

/* variable */
#if LOG_LEVEL >= __ERROR__
    #define __LOG_VAR__(var_name, format, ...) \
            __OUTPUT__(__AS_VAR__(var_name, format), __LINE__, ##__VA_ARGS__)
#else
    #define __LOG_VAR__(var_name, format, ...) ((void)0)
#endif

/* trace time */
#if LOG_LEVEL >= __ERROR__
    #define __LOG_TRACE__(code, tag) \
    do \
    { \
        struct timeval __start, __end; \
        gettimeofday(&__start, NULL); \
        code \
        gettimeofday(&__end, NULL); \
        long long __time_cost = (__end.tv_sec - __start.tv_sec) * 1e6 + (__end.tv_usec - __start.tv_usec); \
        __OUTPUT__("[%s] cost time = %fs\n", tag, (double)__time_cost / 1e6); \
    } while (0);
#else
    #define __LOG_TRACE__(code, tag) ((void)0)
#endif


#endif