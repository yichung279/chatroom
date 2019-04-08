#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <unistd.h>

int main(){

    struct sockaddr_in ser_addr;
    int addr_len = sizeof(ser_addr);
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");;
    ser_addr.sin_port = htons(18700);
    memset(&ser_addr.sin_zero[0], 0, 8);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1){
        printf("Fail to create a socket.");
        exit(1);
    }
    connect(sockfd, (struct sockaddr *)&ser_addr, addr_len);

    char message[256];
    while(1){
        fgets(message, 256, stdin);
        printf("%s", message);
        if (strcmp(message, "exit\n") == 0)
            break;
        if (strcmp(message, "") != 0){
            send(sockfd, message, sizeof(message), 0);
            printf("Socket send: %s\n", message);
            memset(message, 0, 256);
        }

        char receiveMessage[100] = {};
        int n = recv(sockfd, receiveMessage, sizeof(receiveMessage), 0);
        if (n > 0)
            printf("%s",receiveMessage);

    }
    close(sockfd);
    return 0;
}

