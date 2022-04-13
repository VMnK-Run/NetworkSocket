#include "process.h"

int process(Request *request, char *buf, int readret){

    //400
    if(request == NULL) {
        strcpy(buf, Bad_request_400);
        return strlen(Bad_request_400);
    }

    //505
    if(strcmp(request->http_version, "HTTP/1.1") != 0) {
        strcpy(buf, RESPONSE_505);
        return strlen(RESPONSE_505);
    }

    if(strcmp(request->http_method, "GET") == 0) {
        char url[BUF_SIZE];
        if(strcmp(request->http_uri, "/") == 0) {
            strcpy(url, Default_URI);
        } else {
            strcpy(url, request->http_uri);
        }

        int fd_in = open(url, O_RDONLY);
        if(fd_in < 0) {
            strcpy(buf, RESPONSE_404);
            return strlen(RESPONSE_404);
        }
        memset(buf, 0, sizeof(buf));
        strcpy(buf, HTTP_1_1_200_OK);
        char temp[BUF_SIZE - strlen(buf)];
        int readRet = read(fd_in, temp, BUF_SIZE - strlen(buf));
        strncat(buf, temp, readret);
        return strlen(buf);
    }

    if(strcmp(request->http_method, "POST") == 0) {
        return readret;
    }

    if(strcmp(request->http_method, "HEAD") == 0) {
        strcpy(buf, HTTP_1_1_200_OK);
        return strlen(HTTP_1_1_200_OK);
    }

    //501
    strcpy(buf, Not_Implemented);
    return strlen(Not_Implemented);
}