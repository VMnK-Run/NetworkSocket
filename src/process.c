#include "process.h"

int process(Request *request, char *buf, int readret){

    //400
    if(request == NULL) {
        strcpy(buf, Bad_request_400);
        ACCESS_NULL();
        ACCESS_NULL_TO_FILE();
        ERROR("Bad request!");
        ERROR_TO_FILE("Bad request!");
        return strlen(Bad_request_400);
    }

    //505
    if(strcmp(request->http_version, "HTTP/1.1") != 0) {
        strcpy(buf, RESPONSE_505);
        ACCESS(request, 505, strlen(RESPONSE_505));
        ACCESS_TO_FILE(request, 505, strlen(RESPONSE_505));
        return strlen(RESPONSE_505);
    }

    if(strcmp(request->http_method, "GET") == 0) {
        char url[BUF_SIZE];
        if(strcmp(request->http_uri, "/") == 0) {
            strcpy(request->http_uri, Default_URI);
            strcpy(url, Default_URI);
        } else {
            strcpy(url, request->http_uri);
        }

        int fd_in = open(url, O_RDONLY);
        if(fd_in < 0) {
            strcpy(buf, RESPONSE_404);
            ACCESS(request, 404, strlen(RESPONSE_404));
            ACCESS_TO_FILE(request, 404, strlen(RESPONSE_404));
            return strlen(RESPONSE_404);
        }
        memset(buf, 0, sizeof(buf));
        strcpy(buf, HTTP_1_1_200_OK);
        char temp[BUF_SIZE];
        int len = BUF_SIZE - strlen(buf);
        int readRet = read(fd_in, temp, len);
        strncat(buf, temp, readRet);
        ACCESS(request, 200, strlen(buf));
        ACCESS_TO_FILE(request, 200, strlen(buf));
        return strlen(buf);
    }

    if(strcmp(request->http_method, "POST") == 0) {
        ACCESS(request, 200, readret);
        ACCESS_TO_FILE(request, 200, readret);
        return readret;
    }

    if(strcmp(request->http_method, "HEAD") == 0) {
        strcpy(buf, HTTP_1_1_200_OK);
        ACCESS(request, 200, strlen(HTTP_1_1_200_OK));
        ACCESS_TO_FILE(request, 200, strlen(HTTP_1_1_200_OK));
        return strlen(HTTP_1_1_200_OK);
    }

    //501
    strcpy(buf, Not_Implemented);
    ACCESS(request, 505, strlen(Not_Implemented));
    ACCESS_TO_FILE(request, 505, strlen(Not_Implemented));
    ERROR("The Method %s has not been implemented!", request->http_method);
    ERROR_TO_FILE("The Method %s has not been implemented!", request->http_method);
    return strlen(Not_Implemented);
}