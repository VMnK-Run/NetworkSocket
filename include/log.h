#ifndef __LOG_H__
#define __LOG_H__

/**
 * Define Log Level Here
 * 0: 没有log
 * 1: 只输出Error
 * 2: 输出Error和Warning
 * 3: 输出所有类型的log
*/
#define LOG_LEVEL 3

/*Log Levels*/
#define	__NONE__		0
#define	__ERROR__		1
#define __WARNING__	2
#define __MESSAGE__ 3


#define ERROR(format, ...)		__LOG_ERROR__(format, ##__VA_ARGS__)
#define WARNING(format, ...)	__LOG_ERROR__(format, ##__VA_ARGS__)
#define MSG(format, ...)			__LOG_MSG__(format, ##__VA_ARGS__)

/*程序所在文件和行数*/
#define __WHERE__							__FILE__ ": " "%d "


#if LOG_LEVEL >= __ERROR__
    #define __LOG__ERROR__(format, ...) \
            
#else

#endif


#endif