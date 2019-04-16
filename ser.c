#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <sys/select.h>

#include "q_conn.c"

#define MESSAGE_MAX_SIZE 256+12

void boardcast(q_conn *conn_list, char message[MESSAGE_MAX_SIZE]);

int main(){
    // build server socket
    struct sockaddr_in ser_addr, cli_addr;
    int addr_len = sizeof(ser_addr);

    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = INADDR_ANY;
    ser_addr.sin_port = htons(18700);
    memset(&ser_addr.sin_zero[0], 0, 8);

    int serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd == -1){
        printf("Fail to create a socket.");
        exit(1);
    }

    bind(serverfd, (struct sockaddr *)&ser_addr, addr_len);
    listen(serverfd,5);

    fd_set master;
    fd_set read_fds;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    FD_SET(serverfd, &master);
    int fdmax = serverfd;

    q_conn * user_list = q_conn_new();
    char message[MESSAGE_MAX_SIZE] = {};

    while(1) {
        read_fds = master;// update every loop


        if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1){
            printf("ERROR: server select failed\n");
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i <= fdmax; ++i){
            if (!FD_ISSET(i, &read_fds)) continue;

            // sever check new connection
            if(i == serverfd){
                int userfd = accept(serverfd, (struct sockaddr *)NULL, NULL);
                if (userfd == -1) continue;

                q_conn_insert(user_list, userfd);
                FD_SET(userfd, &master);
                if (userfd > fdmax)
                    fdmax = userfd;
            }

            // client check message recving
            else{
                int n = recv(i, message, MESSAGE_MAX_SIZE, 0);
                if (n>0){
                    boardcast(user_list, message);
                }
                else{
                    close(i);
                    FD_CLR(i, &master);
                }
                memset(message, 0, MESSAGE_MAX_SIZE);
            }
        }
    }
    return 0;
}

void boardcast(q_conn *conn_list, char message[MESSAGE_MAX_SIZE]){
    n_conn *current = conn_list->head;
    while(current){
        send(current->conn, message, MESSAGE_MAX_SIZE, 0);
        printf("Socket send: %s\n", message);
        current = current->next;
    }
}
