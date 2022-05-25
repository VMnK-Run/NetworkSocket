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
#include <sys/select.h>
#include <time.h>
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
    fd_set readfds;
    int client_nums = 1;
    char ip_addresses[1024][32];

    fprintf(stdout, "----- Liso Server -----\n");
    initLog();
    /* all networked programs must create a socket */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        //fprintf(stderr, "Failed creating socket.\n");
        ERROR("Failed creating socket.\n");
        ERROR_TO_FILE("Failed creating socket.\n");
        return EXIT_FAILURE;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(ECHO_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    /* servers bind sockets to ports---notify the OS they accept connections */
    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)))
    {
        close_socket(sock);
        //fprintf(stderr, "Failed binding socket.\n");
        ERROR("Failed binding socket.");
        ERROR_TO_FILE("Failed binding socket.");
        return EXIT_FAILURE;
    }

    if (listen(sock, 5))//开启监听
    {
        close_socket(sock);
        //fprintf(stderr, "Error listening on socket.\n");
        ERROR("Error listening on socket.");
        ERROR_TO_FILE("Error listening on socket.");
        return EXIT_FAILURE;
    }

    FD_ZERO(&readfds);

    /* finally, loop waiting for input and then write it back */
    while (1)
    {
        LOG("liso server waiting");

        FD_SET(sock, &readfds); // 这句一定要放在这里
        int select_num = select(MAXFDS, &readfds, NULL, NULL, NULL);
        LOG("select:%d", select_num);
        if(select_num < 0) {
            LOG_ERROR("select wrong! %d", select_num);
            return EXIT_FAILURE;
        }

        for(int fd = 0; fd < MAXFDS; fd++) {//遍历看哪个发生了变化
            if(!FD_ISSET(fd, &readfds)) continue; //读变化
            if(fd == sock) { //获得新连接
                cli_size = sizeof(cli_addr);
                //建立连接
                if ((client_sock = accept(sock, (struct sockaddr *) &cli_addr,
                                            &cli_size)) == -1)
                {
                    close(sock);
                    //fprintf(stderr, "Error accepting connection.\n");
                    ERROR("Error accepting connection.");
                    ERROR_TO_FILE("Error accepting connection.");
                    return EXIT_FAILURE;
                }

                if(client_nums < MAXFDS) {
                    client_nums++;
                    FD_SET(client_sock, &readfds);
                    readret = 0;
                    memset(ip_address, 0, sizeof(ip_address));
                    strcpy(ip_address, inet_ntoa(cli_addr.sin_addr));
                    strcpy(ip_addresses[client_sock], ip_address);
                    MSG("connected the client on fd %d", client_sock);
                } else {
                    LOG_ERROR("client nums full!");
                }
                LOG("new connect tcp！");
            } else { //获得新请求
                client_sock = fd;
                memset(ip_address, 0, sizeof(ip_address));
                strcpy(ip_address, ip_addresses[fd]);
                //接受数据
                if ((readret = recv(client_sock, buf, BUF_SIZE, 0)) >= 1)
                {   
                    //fprintf(stderr, "i receive data: %d\n", readret);
                    //原本是从buf读进来,再用buf读回去,所以需要作处理

                    int *idx = (int *)malloc(sizeof(int));
                    *idx = 0;

                    while(*idx < readret) {

                        int temp = *idx;
                        Request *request = parse(buf + *idx, readret - *idx, sock, idx);
                        if(request == NULL && *idx - temp == 2) continue;
                        char res[BUF_SIZE * 24];
                        strncpy(res, buf + temp, *idx - temp);
                        int readRet = process(request, res, *idx - temp);
                        if (send(client_sock, res, readRet, 0) != readRet)
                        {
                            close_socket(client_sock);
                            close_socket(sock);
                            ERROR("Error sending to client.");
                            ERROR_TO_FILE("Error sending to client.");
                            return EXIT_FAILURE;
                        }
                    }

                    memset(buf, 0, BUF_SIZE);

                //想使用apache bench吗？把下面这个else注释掉就可以了
                } else {
                    FD_CLR(client_sock, &readfds);
                    client_nums--;
                    close_socket(client_sock);
                    if (readret == -1)
                    {
                        close_socket(client_sock);
                        close_socket(sock);
                        //fprintf(stderr, "Error reading from client socket.\n");
                        ERROR("Error reading from client socket.");
                        ERROR_TO_FILE("Error reading from client socket.");
                        return EXIT_FAILURE;
                    }
                }

            }
        }

    }

    closeLog();
    close_socket(sock);

    return EXIT_SUCCESS;
}
