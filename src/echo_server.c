/******************************************************************************
* echo_server.c                                                               *
*                                                                             *
* Description: This file contains the C source code for an echo server.  The  *
*              server runs on a hard-coded port and simply write back anything*
*              sent to it by connected clients.  It does not support          *
*              concurrent clients.                                            *
*                                                                             *
* Authors: Athula Balachandran <abalacha@cs.cmu.edu>,                         *
*          Wolf Richter <wolf@cs.cmu.edu>                                     *
*                                                                             *
*******************************************************************************/

#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "parse.h"
#include "process.h"
#include "params.h"
#include "log.h"

#define ECHO_PORT 9999
//#define BUF_SIZE 4096

int close_socket(int sock)
{
    if (close(sock))
    {
        fprintf(stderr, "Failed closing socket.\n");
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    int sock, client_sock;
    ssize_t readret;
    socklen_t cli_size;
    struct sockaddr_in addr, cli_addr;
    char buf[BUF_SIZE * 24];

    fprintf(stdout, "----- Liso Server -----\n");
    initLog();
    /* all networked programs must create a socket */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "Failed creating socket.\n");
        return EXIT_FAILURE;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(ECHO_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    /* servers bind sockets to ports---notify the OS they accept connections */
    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)))
    {
        close_socket(sock);
        fprintf(stderr, "Failed binding socket.\n");
        return EXIT_FAILURE;
    }

    if (listen(sock, 5))//开启监听
    {
        close_socket(sock);
        fprintf(stderr, "Error listening on socket.\n");
        return EXIT_FAILURE;
    }

    /* finally, loop waiting for input and then write it back */
    while (1)
    {
        cli_size = sizeof(cli_addr);
        //建立连接
        if ((client_sock = accept(sock, (struct sockaddr *) &cli_addr,
                                    &cli_size)) == -1)
        {
            close(sock);
            fprintf(stderr, "Error accepting connection.\n");
            return EXIT_FAILURE;
        }

        readret = 0;
        memset(ip_address, 0, sizeof(ip_address));
        strcpy(ip_address, inet_ntoa(cli_addr.sin_addr));

        //接受数据
        while((readret = recv(client_sock, buf, BUF_SIZE, 0)) >= 1)
        {   
            //fprintf(stderr, "i receive data: %d\n", readret);
            //原本是从buf读进来,再用buf读回去,所以需要作处理

            int *idx = (int *)malloc(sizeof(int));
            *idx = 0;
            int length = 0;
            char res[BUF_SIZE * 24];
            fprintf(stderr, "readret: %d\n", readret);
            while(*idx < readret) {
                fprintf(stderr, "idx: %d\n", *idx);
                int temp = *idx;
                Request *request = parse(buf + *idx, readret - *idx, sock, idx);
                //if(request == NULL && *idx - temp == 2) continue;
                char temp_buf[BUF_SIZE * 24];
                strncpy(temp_buf, buf + temp, *idx - temp);
                fprintf(stderr, "buf:\n%s\n", temp_buf);
                int readRet = process(request, temp_buf, *idx - temp);
                fprintf(stderr,"after buf:\n%s\n", temp_buf);
                length += readRet;
                strncat(res, temp_buf, readRet);
            }
            fprintf(stderr, "readret: %d\n", readret);
            fprintf(stderr, "result:\n%s\n", res);
            if (send(client_sock, res, length, 0) != length)
            {
                close_socket(client_sock);
                close_socket(sock);
                fprintf(stderr, "Error sending to client.\n");
                return EXIT_FAILURE;
            }
            memset(buf, 0, BUF_SIZE);
            memset(res, 0, sizeof(res));
        } 

        if (readret == -1)
        {
            close_socket(client_sock);
            close_socket(sock);
            fprintf(stderr, "Error reading from client socket.\n");
            return EXIT_FAILURE;
        }

        if (close_socket(client_sock))
        {
            close_socket(sock);
            fprintf(stderr, "Error closing client socket.\n");
            return EXIT_FAILURE;
        }
    }

    closeLog();
    close_socket(sock);

    return EXIT_SUCCESS;
}
