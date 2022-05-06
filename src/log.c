#include "log.h"

char* getTime(int i) {
    memset(curtime, 0, sizeof(curtime));
	time_t time0;
	time(&time0);
	struct tm* l_time = localtime(&time0);
    if(i == 0) strftime(curtime, 30, "%a %b %d %X %Y", l_time);
    else if(i == 1) {
        strftime(curtime, 30, "%d/%b/%Y:%X", l_time);
    }
    return curtime;
}

void initLog() {
    FILE *error = fopen(ERROR_FILE, "a+");
    if(error == NULL) {
        LOG_ERROR("Failed to open error_log.txt!");
        exit(0);
    }
    else {
        LOG("Open error_log.txt succesfully!");
    }
    fprintf(error, "--Liso Server-- begin at [%s]\n", getTime(0));
    fclose(error);

    FILE *access = fopen(ACCESS_FILE, "a+");
    if(access == NULL) {
        LOG_ERROR("Failed to open access_log.txt!");
        exit(0);
    }
    else {
        LOG("Open access_log.txt succesfully!");
    }
    fprintf(access, "--Liso Server-- Begin at [%s]\n", getTime(0));
    fclose(access);
}

void closeLog() {
    FILE *error = fopen(ERROR_FILE, "a+");
    fprintf(error, "--Liso Server-- End at [%s]\n", getTime(0));
    fclose(error);
    FILE *access = fopen(ACCESS_FILE, "a+");
    fprintf(access, "--Liso Server-- End at [%s]\n", getTime(0));
    fclose(access);
}