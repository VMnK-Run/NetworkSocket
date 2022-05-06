#ifndef __PARAMS_H__
#define __PARAMS_H__

#define BUF_SIZE 8192
#define LINES 20

#define ERROR_FILE       "log/error_log.txt"
#define ACCESS_FILE      "log/access_log.txt"

#define Bad_request_400 "HTTP/1.1 400 Bad request\r\n\r\n"
#define Not_Implemented "HTTP/1.1 501 Not Implemented\r\n\r\n"
#define HTTP_1_1_200_OK "HTTP/1.1 200 OK\r\n\r\n"
#define RESPONSE_505 "HTTP/1.1 505 HTTP Version not supported\r\n\r\n"
#define RESPONSE_404 "HTTP/1.1 404 Not Found\r\n\r\n"
#define Default_URI "static_site/index.html"


#endif