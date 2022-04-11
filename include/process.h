#ifndef __PROCESS_H_
#define __PROCESS_H_

#include "parse.h"

#define Bad_request "HTTP/1.1 400 Bad request\r\n\r\n"
#define Not_Implemented "HTTP/1.1 501 Not Implemented\r\n\r\n"
#define HTTP_1_1_200_OK "HTTP/1.1 200 OK\r\n\r\n"


int process(Request *, char *, int);

#endif