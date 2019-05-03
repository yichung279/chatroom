#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main(){

    char inputBuffer[256] = {};
    char tmp[267] = {};
    char message[] = {"Server got: "};
    struct sockaddr_in ser_addr, cli_addr;
    int addr_len = sizeof(ser_addr);

    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = INADDR_ANY;
    ser_addr.sin_port = htons(18700);
    memset(&ser_addr.sin_zero[0], 0, 8);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1){
        printf("Fail to create a socket.");
        exit(1);
    }
    bind(sockfd, (struct sockaddr *)&ser_addr, addr_len);
    listen(sockfd,5);
    int conn;
    while(conn = accept(sockfd, (struct sockaddr *)NULL, NULL)) {
        int pid;
        if((pid = fork()) != -1) {
            while (recv(conn, inputBuffer, sizeof(inputBuffer), 0)>0) {
                printf("Get:%s\n",inputBuffer);
                strcpy(tmp, message);
                strcat(tmp, inputBuffer);
                send(conn, tmp, sizeof(tmp), 0);
                memset(inputBuffer, 0, sizeof(inputBuffer));
                memset(tmp, 0, sizeof(tmp));
            }
            exit(0);
        }
     }
    return 0;
}
