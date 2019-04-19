#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define MESSAGE_MAX_SIZE 256
char* concat(char *, char *);

int main(){
    char user[10]={};
    system("clear");
    printf("Your nickname: ");
    scanf("%s", user);


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

    fd_set read_fds;
    char message[MESSAGE_MAX_SIZE] = {};
    char receive_message[MESSAGE_MAX_SIZE] = {};
    char msg_list[1000][MESSAGE_MAX_SIZE];
    int msg_list_size = 0;

    char *join_message = malloc(MESSAGE_MAX_SIZE);
    strcpy(join_message, user);
    strcat(join_message, " join chatroom ");

    send(sockfd, join_message, MESSAGE_MAX_SIZE, 0);

    while(1) {
        FD_ZERO(&read_fds);
        FD_SET(0, &read_fds);
        FD_SET(sockfd, &read_fds);

        if(select(sockfd+1, &read_fds, NULL, NULL, NULL) == -1){
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(0, &read_fds)){
            fgets (message, MESSAGE_MAX_SIZE, stdin);

            char *send_message = malloc(MESSAGE_MAX_SIZE);
            strcpy(send_message, user);
            strcat(send_message, ": ");
            if(strcmp(message, "\n") == 0) {
                continue;
            }
            strcat(send_message, message);

            send(sockfd, send_message, MESSAGE_MAX_SIZE, 0);
            memset(message, 0, MESSAGE_MAX_SIZE);
        }

        if (FD_ISSET(sockfd, &read_fds)){
            int n = recv(sockfd, receive_message, MESSAGE_MAX_SIZE, 0);
            strcpy(msg_list[msg_list_size], receive_message);
            memset(receive_message, 0, MESSAGE_MAX_SIZE);
            msg_list_size++;

            system("clear");
            for (int i = 0; i < msg_list_size; ++i)
                printf("%s\n", msg_list[i]);

        }
    }
    close(sockfd);
    return 0;
}
