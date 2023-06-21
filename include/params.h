#ifndef __PARAMS_H__
#define __PARAMS_H__


#define Bad_request "HTTP/1.1 400 Bad request\r\n\r\n"
#define Not_Implemented "HTTP/1.1 501 Not Implemented\r\n\r\n"
#define HTTP_1_1_200_OK "HTTP/1.1 200 OK\r\n\r\n"

static const int NUM_HEAD_LINES = 6;
static const int BUF_SIZE = 4096;
static const int ECHO_PORT = 9999;

#endif