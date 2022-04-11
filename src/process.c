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
        return readret;
    }

    strcpy(buf, Not_Implemented);
    return strlen(Not_Implemented);
}