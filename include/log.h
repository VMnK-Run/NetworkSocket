#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "params.h"

char ip_address[32];
char curtime[1024];
FILE *file;

char *getTime(int i);
void initLog();
void closeLog();

#define __OUTPUT__(format, ...)         fprintf(stdout, format, ##__VA_ARGS__)
#define __TIME_IP__                     getTime(0), ip_address 
#define __WHERE__                       __FILE__, __LINE__                    

#define LOG(format, ...)                __OUTPUT__(__AS_LOG__(format, ##__VA_ARGS__))
#define LOG_ERROR(format, ...)          __OUTPUT__(__AS_LOG_ERROR__(format, ##__VA_ARGS__))
#define ERROR(format, ...)              __OUTPUT__(__AS_ERROR__(format, ##__VA_ARGS__))
#define ACCESS(request, state, length)  __OUTPUT__(__AS_ACCESS__(request, state, length))
#define ACCESS_NULL()                   __OUTPUT__(__AS_ACCESS_NULL__())
#define MSG(format, ...)                __OUTPUT__(__AS_MSG__(format,  ##__VA_ARGS__))

#define ERROR_TO_FILE(format, ...)                 __LOG_ERROR_TO_FILE__(format, ##__VA_ARGS__)
#define ACCESS_TO_FILE(request, state, length)     __LOG_ACCESS_TO_FILE__(request, state, length)
#define ACCESS_NULL_TO_FILE()                      __LOG_ACCESS_NULL_TO_FILE__()

#define __AS_LOG__(format, ...) \
        format "\n", ##__VA_ARGS__

#define __AS_LOG_ERROR__(format, ...) \
        "\033[1;31m" "[Error!]" "\033[0m" format ": %s: %d\n",\
        ##__VA_ARGS__, __WHERE__

#define __AS_ERROR__(format, ...) \
        "[%s]" "\033[1;31m" "[Error]" "\033[0m[client %s] " format ": %s: %d\n", \
        __TIME_IP__, ##__VA_ARGS__, __WHERE__

#define __AS_ACCESS__(request, state, length) \
        "%s - - [%s +0800] " "\"%s %s %s \" " "%d %d\n", \
        ip_address, getTime(1), request->http_method, request->http_uri, request->http_version, state, length

#define __AS_ACCESS_NULL__() \
        "%s - - [%s +0800] " "\"%s\"" " %d %d\n", \
        ip_address, getTime(1), "Bad_request", 400, strlen("Bad_request")

#define __AS_MSG__(format, ...) \
        "[%s]" "\033[1;32m" "[Debug]" "\033[0m" "%s:%d " format "\n", \
        getTime(0), __WHERE__, ##__VA_ARGS__

#define __LOG_ERROR_TO_FILE__(format, ...) \
        do { \
                file = fopen(ERROR_FILE, "a+");\
                fprintf(file, "[%s][Error][client %s]" format ": %s: %d\n", __TIME_IP__, ##__VA_ARGS__, __WHERE__);\
                fclose(file);\
        } while(0)

#define __LOG_ACCESS_TO_FILE__(request, state, length) \
        do { \
                file = fopen(ACCESS_FILE, "a+");\
                fprintf(file, __AS_ACCESS__(request, state, length));\
                fclose(file);\
        } while(0)

#define __LOG_ACCESS_NULL_TO_FILE__() \
        do {\
                file = fopen(ACCESS_FILE, "a+");\
                fprintf(file, __AS_ACCESS_NULL__());\
                fclose(file);\
        } while(0)
#endif