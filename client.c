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
<<<<<<< HEAD
#include "split.h"
#include "stdlib.h"

//sem_t semaphore;
unsigned char password[40];

unsigned char *xor(unsigned char *message, unsigned char *password) {
    unsigned char *xorMessage = malloc(strlen(message) + 1);
    size_t        length      = strlen(message);

    for (int i = 0; i < length; ++i) {
        xorMessage[i] = (message[i] ^ password[i]);
    }
    return xorMessage;
}

unsigned char swapNibbles(unsigned char x) {
    return ((x & 0x0F) << 4 | (x & 0xF0) >> 4);
}

unsigned char *charToHex(char *txt, int control) {
    unsigned char *hexTxt = calloc(2 * strlen(txt) + 1, 1);
    unsigned char *p;
    for (p = hexTxt; *txt; txt++, p += 2) {
        if (control == 0) {
            sprintf(p, "%0x", swapNibbles(*txt));
        } else {
            sprintf(p, "%0x", *txt);
        }
    }
    return hexTxt;
}

char *encode(unsigned char *message, unsigned char password[40]) {

    unsigned char *hexMessage      = charToHex(message, 1);
    int           lengthOfPassword = 0;
    int           control          = 0;

    lengthOfPassword = strlen(message);

    if (lengthOfPassword > strlen(password)) {
        control = 1;
    }

    unsigned char *adaptedPassword = malloc(lengthOfPassword);

    for (int i = 0; i < lengthOfPassword; ++i) {
        if (control == 0) {
            adaptedPassword[i] = password[i];
        } else {
            adaptedPassword[i] = password[i % 40];
        }
    }

    unsigned char *hexPassword = charToHex("fbx", 1);

    unsigned char *output = xor(hexMessage, hexPassword);

    return output;
}

void decode(char *message, char *password) {
//TODO :(
}

void *sendMessage(void *sock_desc) {
    //Send some data
    int control = 0;

    printf("\n\033[34;1m Your commands are 'login <userName>,'getusers','alias <groupName> <person1,person2...>'\033[0m\n\n");
    while (1) {

        unsigned char message[2000];

        scanf("%[^\n]%*c", message);
        fflush(stdin);

        if (strcmp(message, "exit") == 0) {
            close(*(int *) sock_desc);
            exit(0);
        }

        encode(message, password); //THIS FUNCTION DOESNT WORK

=======

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

>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd
        if (send(*((int *) sock_desc), message, strlen(message) + 1, 0) < 0) {
            puts("Send failed");
            exit(1);
        }
<<<<<<< HEAD
=======
        //sleep(1);
>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd

    }
}

void *receiveMessage(void *sock_desc) {
<<<<<<< HEAD

    char *parsedMessage[30];
    while (1) {
=======
    while (1) {
        //sem_wait(&semaphore);
>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd

        char server_reply[2000];
        if (recv(*((int *) sock_desc), server_reply, 2000, 0) < 0) {
            puts("recv failed");
            exit(1);
        }
<<<<<<< HEAD
        char *tempServer_reply = server_reply;
        parsing(parsedMessage, tempServer_reply, "*");
        if (strcmp(parsedMessage[0], "password") == 0) {
            strcpy(password, parsedMessage[1]);
        } else {
            printf("\033[32;1m%s\033[0m\n", server_reply);
        }
=======

        //Receive a reply from the server
        printf("\033[32;1m %s \033[0m\n", server_reply);

        //sem_post(&semaphore);
>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd
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

<<<<<<< HEAD
    pthread_create(&receive_thread, NULL, receiveMessage, (void *) new_sock);
    pthread_create(&send_thread, NULL, sendMessage, (void *) new_sock);


=======
    //sem_init(&semaphore, 0, 1);
    //pthread_mutex_init(&lock, NULL);


    pthread_create(&send_thread, NULL, sendMessage, (void *) new_sock);
    pthread_create(&receive_thread, NULL, receiveMessage, (void *) new_sock);

>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd
    pthread_join(send_thread, NULL);
    pthread_join(receive_thread, NULL);


    close(sock);
    return 0;
<<<<<<< HEAD
}
=======
}
>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd
