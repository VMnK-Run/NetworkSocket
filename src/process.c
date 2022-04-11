#include "process.h"

int process(Request *request, char *buf, int readret){
    if(request == NULL) {
        strcpy(buf, Bad_request);
        return strlen(Bad_request);
    }

    if(strcmp(request->http_method, "GET") == 0) {
        return readret;
    }

    if(strcmp(request->http_method, "POST") == 0) {
        return readret;
    }

    if(strcmp(request->http_method, "HEAD") == 0) {
        strcpy(buf, HTTP_1_1_200_OK);
        return strlen(HTTP_1_1_200_OK);
    }

    strcpy(buf, Not_Implemented);
    return strlen(Not_Implemented);
}