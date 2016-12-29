//
// Created by berkin on 18.12.2016.
//

#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h> // for inet_addr
#include <string.h>
#include <zconf.h>
#include <stdlib.h>
#include <pthread.h>
#include "semaphore.h"

//sem_t semaphore;
void *sendMessage(void *sock_desc) {
    //Send some data

    while (1) {
        //sem_wait(&semaphore);

        char message[2000];
        printf("> ");
        //sem_post(&semaphore);
        scanf("%[^\n]%*c", message);
        fflush(stdin);

        if (strcmp(message, "exit") == 0) {
            close(*(int *)sock_desc);
            exit(0);
        }

        if (send(*((int *) sock_desc), message, strlen(message) + 1, 0) < 0) {
            puts("Send failed");
            exit(1);
        }
        //sleep(1);

    }
}

void *receiveMessage(void *sock_desc) {
    while (1) {
        //sem_wait(&semaphore);

        char server_reply[2000];
        if (recv(*((int *) sock_desc), server_reply, 2000, 0) < 0) {
            puts("recv failed");
            exit(1);
        }

        //Receive a reply from the server
        printf("\033[32;1m %s \033[0m\n", server_reply);

        //sem_post(&semaphore);
    }
}

int main(int argc, char *argv[]) {
    int                sock;
    struct sockaddr_in server;

    //Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Could not create socket");
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family      = AF_INET;
    server.sin_port        = htons(8888);

    //Connect to remote server
    if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("Connect failed. Error");
        return 1;
    }

    puts("Connected to server\n");
    int *new_sock;
    new_sock = malloc(sizeof(int));
    *new_sock = sock;
    //keep communicating with server

    pthread_t send_thread, receive_thread;

    //sem_init(&semaphore, 0, 1);
    //pthread_mutex_init(&lock, NULL);


    pthread_create(&send_thread, NULL, sendMessage, (void *) new_sock);
    pthread_create(&receive_thread, NULL, receiveMessage, (void *) new_sock);

    pthread_join(send_thread, NULL);
    pthread_join(receive_thread, NULL);


    close(sock);
    return 0;
}