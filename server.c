#include<stdio.h>
#include<string.h>    // for strlen
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h> // for inet_addr
#include<unistd.h>    // for write
#include<pthread.h>   // for threading, link with lpthread
#include "split.h"

#define MAX_CLIENT_NUMBER 100

void *connection_handler(void *);

struct User searchUser(char searchName[10]);


struct User {
    char userName[10];
    int  clientSocketNo;
};

struct Group {
    char        groupName[20];
    struct User person[10];
    int         groupSize;
};

unsigned int clientNumber    = 0;
unsigned int userArrayIndex  = 0;
unsigned int groupArrayIndex = 0;
struct User  users[MAX_CLIENT_NUMBER];
struct Group group[20];

//FOR GETUSERS COMMAND
void getUsers(int socketNumber) {
    char *userString = malloc(100);

    if (userArrayIndex > 0) {
        strcpy(userString, users[0].userName);
        strcat(userString, ",");
        for (int i = 1; i < MAX_CLIENT_NUMBER; ++i) {
            if(strcmp(users[i].userName,"") != 0){
                strcat(userString, users[i].userName);
                strcat(userString, ",");
            }
        }
    }
    else {
        userString = "There is no user";
        write(socketNumber, userString, strlen(userString) + 1);
        free(userString);
        return;
    }
    write(socketNumber, userString, strlen(userString) + 1);
    free(userString);
}

//AFTER LOGIN COMMAND, ADD TO THE ARRAY
void addUserToArray(char userName[10], int socketNumber) {
    printf("Client logged in as %s\n", userName);
    strcpy(users[userArrayIndex].userName, userName);
    users[userArrayIndex].clientSocketNo = socketNumber;
    userArrayIndex++;
}

//LOGIN COMMAND
void loginUser(char userName[10], int socketNumber) {
    char *message = "login successful";
    write(socketNumber, message, strlen(message) + 1);
    addUserToArray(userName, socketNumber);
}

//SEND MESSAGE IF USER FOUND
void sendMessage(int clientSocketNumber, char *message) {
    write(clientSocketNumber, message, strlen(message) + 1);
}

//SEND MESSAGE IF GROUP FOUND
void sendMessageToGroup(struct Group group, char *message) {
    for (int i = 0; i < group.groupSize; ++i) {
        write(group.person[i].clientSocketNo, message, strlen(message) + 1);
    }
}

//ADD GROUP TO ARRAY
void addGroupToArray(char groupName[20], char *persons) {
    char *parsedInput[10];
    int  numberOfPerson = 0;

    strcpy(group[groupArrayIndex].groupName, groupName);
    numberOfPerson = parsing(parsedInput, persons, ",");

    for (int i = 0; i < numberOfPerson; ++i) {
        group[groupArrayIndex].person[i] = searchUser(parsedInput[i]);
    }

    group[groupArrayIndex].groupSize = numberOfPerson;
    groupArrayIndex++;
}

//CREATE GROUP AND CALL ADD-GROUP-TO-ARRAY FUNCTION
void createGroup(char *groupName, char *persons, int socketNumber) {
    char *message = "Group crated successfully";
    write(socketNumber, message, strlen(message) + 1);
    addGroupToArray(groupName, persons);
}

//SEARCH USER FROM ARRAY
struct User searchUser(char searchName[10]) {
    for (int i = 0; i < userArrayIndex; ++i) {
        if (strcmp(users[i].userName, searchName) == 0) {
            return users[i];
        }
    }
    struct User null_user;
    strcpy(null_user.userName,"");
    return null_user;
}

//SEARCH GROUP FROM ARRAY
struct Group searchGroup(char groupName[20]) {
    for (int i = 0; i < groupArrayIndex; ++i) {
        if (strcmp(group[i].groupName, groupName) == 0) {
            return group[i];
        }
    }
    struct Group null_grup;
    strcpy(null_grup.groupName,"");
    return null_grup;
}

//IF USER DISCONNECTS, REMOVE
void deleteUser(int socketNo){
    for (int i = 0; i < userArrayIndex; ++i) {
        if (users[i].clientSocketNo == socketNo) {
            users[i].clientSocketNo = -1;
            strcpy(users[i].userName,"");
            userArrayIndex--;
            break;
        }
    }
}

//CHECK USER IS CONNECTED CORRECT & BLOCK MULTIPLE USERS WITH SAME NAME
int checkLogin(char userName[10]) {
    if (strcmp(searchUser(userName).userName, "") == 0) {
        return 0;
    } else
        return 1;
}

//CHECK IS THERE ANY GROUP WITH MULTIPLE NAME
int checkMultiGroup(char groupName[20]) {
    if (strcmp(searchGroup(groupName).groupName, "") == 0) {
        return 0;
    } else
        return -1;
}

//THREAD?
void *receivedMessage(void *sock){
    char receivedMessage[2000];  //client's message
    int  isLogged         = -1;
    int  readControl;
    int  parsedItemNumber = 0;

    int new_sock = *((int *)sock);
    while ((readControl = recv(new_sock, receivedMessage, 2000, 0)) > 0) {
        char *parsedCommand[50]; //parsedClientMessage

        parsedItemNumber = parsing(parsedCommand, receivedMessage, " ");

        if (strcmp(parsedCommand[0], "login") == 0 && parsedCommand[1] != NULL) {
            if (isLogged != 0)
                isLogged = checkLogin(parsedCommand[1]);

            else{
                sendMessage(new_sock, "One login in one client");
                continue;
            }

            if (isLogged != 1)
                loginUser(parsedCommand[1], new_sock);

            else
                sendMessage(new_sock, "You can login only once");


        } else if (isLogged != -1 & strcmp(parsedCommand[0], "getusers") == 0) {
            getUsers(new_sock);

        } else if (isLogged != -1 & strcmp(parsedCommand[0], "alias") == 0 && parsedCommand[1] != NULL &&
                   parsedCommand[2] != NULL) {
            int isCreatedBefore = checkMultiGroup(parsedCommand[1]);

            if (isCreatedBefore == 0) {
                createGroup(parsedCommand[1], parsedCommand[2], new_sock);
            } else {
                sendMessage(new_sock, "You can't create same group");
            }
        }
        else {
            if (isLogged == -1) {
                char *errorMessage = "You must login first";
                sendMessage(new_sock, errorMessage);
                printf("hi");
            }

            if (isLogged == 0 && parsedCommand[0] != NULL) {

                struct User  find;
                memset(&find, 0, sizeof(struct User));
                struct Group findGroup;
                memset(&findGroup, 0, sizeof(struct User));

                find= searchUser(parsedCommand[0]);
                findGroup= searchGroup(parsedCommand[0]);
                printf("BURADAYIM %s\n",parsedCommand[0]);
                int control = 0;

                if (strcmp(find.userName, "") != 0) {
                    puts("burası");
                    char *message = malloc(2000);
                    if (parsedCommand[1] != NULL) {
                        strcpy(message, parsedCommand[1]);
                        strcat(message, " ");
                    }
                    for (int i = 2; i < parsedItemNumber; ++i) {
                        strcat(message, parsedCommand[i]);
                        strcat(message, " ");
                    }
                    control = 1;
                    sendMessage(find.clientSocketNo, message);
                }
                if (strcmp(findGroup.groupName, "") != 0) {
                    printf("AHA GRUP ADI SON > %s\n",findGroup.groupName);

                    char *message = malloc(2000);
                    if (parsedCommand[1] != NULL) {
                        strcpy(message, parsedCommand[1]);
                        strcat(message, " ");
                    }
                    for (int i = 2; i < parsedItemNumber; ++i) {
                        strcat(message, parsedCommand[i]);
                        strcat(message, " ");
                    }
                    control = 1;
                    sendMessageToGroup(findGroup, message);
                }
                if(control == 0){
                    puts("burası3");
                    char *errorMessage = "There is no user or group with this name ";
                    sendMessage(new_sock, errorMessage);
                }
            }
        }
    }

    if (readControl == 0) {
        puts("Client disconnected");

        deleteUser(new_sock);
        clientNumber--;
        fflush(stdout);
    } else if (readControl == -1) {
        perror("recv failed");
    }
}

void *connection_handler(void *socket_desc) {
    //Get the socket descriptor

    int  sock             = *((int *) socket_desc);
    char receivedMessage[2000];  //client's message
    int  isLogged         = -1;
    int  readControl;
    int  parsedItemNumber = 0;

    //int new_sock = *((int *)sock);
    while ((readControl = recv(sock, receivedMessage, 2000, 0)) > 0) {
        char *parsedCommand[50]; //parsedClientMessage

        parsedItemNumber = parsing(parsedCommand, receivedMessage, " ");

        if (strcmp(parsedCommand[0], "login") == 0 && parsedCommand[1] != NULL) {
            if (isLogged != 0)
                isLogged = checkLogin(parsedCommand[1]);

            else{
                sendMessage(sock, "One login in one client");
                continue;
            }

            if (isLogged != 1)
                loginUser(parsedCommand[1], sock);

            else
                sendMessage(sock, "You can login only once");


        } else if (isLogged != -1 & strcmp(parsedCommand[0], "getusers") == 0) {
            getUsers(sock);

        } else if (isLogged != -1 & strcmp(parsedCommand[0], "alias") == 0 && parsedCommand[1] != NULL &&
                   parsedCommand[2] != NULL) {
            int isCreatedBefore = checkMultiGroup(parsedCommand[1]);

            if (isCreatedBefore == 0) {
                createGroup(parsedCommand[1], parsedCommand[2], sock);
            } else {
                sendMessage(sock, "You can't create same group");
            }
        }
        else {
            if (isLogged == -1) {
                char *errorMessage = "You must login first";
                sendMessage(sock, errorMessage);
            }

            if (isLogged == 0 && parsedCommand[0] != NULL) {

                struct User  find = searchUser(parsedCommand[0]);
                struct Group findGroup = searchGroup(parsedCommand[0]);

                int control = 0;

                if (strcmp(find.userName, "") != 0) {
                    char *message = malloc(2000);
                    if (parsedCommand[1] != NULL) {
                        strcpy(message, parsedCommand[1]);
                        strcat(message, " ");
                    }
                    for (int i = 2; i < parsedItemNumber; ++i) {
                        strcat(message, parsedCommand[i]);
                        strcat(message, " ");
                    }
                    control = 1;
                    sendMessage(find.clientSocketNo, message);
                }
                if (strcmp(findGroup.groupName, "") != 0) {
                    char *message = malloc(2000);
                    if (parsedCommand[1] != NULL) {
                        strcpy(message, parsedCommand[1]);
                        strcat(message, " ");
                    }
                    for (int i = 2; i < parsedItemNumber; ++i) {
                        strcat(message, parsedCommand[i]);
                        strcat(message, " ");
                    }
                    control = 1;
                    sendMessageToGroup(findGroup, message);
                }
                if(control == 0){
                    char *errorMessage = "There is no user or group with this name ";
                    sendMessage(sock, errorMessage);
                }
            }
        }
    }

    if (readControl == 0) {
        puts("Client disconnected");

        deleteUser(sock);
        clientNumber--;
        fflush(stdout);
    } else if (readControl == -1) {
        perror("recv failed");
    }
    pthread_t send_thread, receive_thread;
    //pthread_create(&send_thread, NULL, sendMessage, (void *) new_sock);
    //pthread_create(&receive_thread, NULL, receivedMessage, socket_desc);

    //pthread_join(send_thread, NULL);
    //pthread_join(receive_thread, NULL);



    //Free the socket pointer
    free(socket_desc);
    return 0;
}

int main(int argc, char *argv[]) {

    int                socket_desc, new_socket, c, *new_sock;
    struct sockaddr_in server, client;

    //Create Socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        puts("Could not create socket");
        return 1;
    }

    server.sin_family      = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port        = htons(8888);

    if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
        puts("Binding failed");
        return 1;
    }

    listen(socket_desc, 3);

    puts("Server started");
    c = sizeof(struct sockaddr_in);

    while ((new_socket = accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &c)) &&
           clientNumber < MAX_CLIENT_NUMBER) {

        pthread_t sniffer_thread[MAX_CLIENT_NUMBER];
        new_sock = malloc(1);
        *new_sock = new_socket;

        if (pthread_create(&sniffer_thread[clientNumber], NULL, connection_handler,
                           (void *) new_sock) < 0) {
            perror("Could not create thread");
            return 1;
        } else {
            clientNumber++;
        }



        puts("Client connected");
    }


    if (new_socket < 0) {
        perror("accept failed");
        return 1;
    }

    return 0;
}