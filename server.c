#include<stdio.h>
#include<string.h>    // for strlen
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h> // for inet_addr
#include<unistd.h>    // for write
#include<pthread.h>   // for threading, link with lpthread
#include "split.h"

#define MAX_CLIENT_NUMBER 100
<<<<<<< HEAD
#define PASSWORD_LENGTH 40
=======
>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd

void *connection_handler(void *);

struct User searchUser(char searchName[10]);

<<<<<<< HEAD
struct User {
    char userName[10];
    int  clientSocketNo;
    char password[40];
=======

struct User {
    char userName[10];
    int  clientSocketNo;
>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd
};

struct Group {
    char        groupName[20];
    struct User person[10];
    int         groupSize;
};

<<<<<<< HEAD
struct Message{
    char message[2000];
    int socketNo;
};

=======
>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd
unsigned int clientNumber    = 0;
unsigned int userArrayIndex  = 0;
unsigned int groupArrayIndex = 0;
struct User  users[MAX_CLIENT_NUMBER];
struct Group group[20];

<<<<<<< HEAD
char *createPassword(int length) {
    char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    size_t stringLen = 26*2+10+7;
    char *randomString;

    randomString = malloc(sizeof(char) * (length +1));

    if (!randomString) {
        return (char*)0;
    }

    unsigned int key = 0;

    for (int n = 0;n < length;n++) {
        key = rand() % stringLen;
        randomString[n] = string[key];
    }

    randomString[length] = '\0';

    return randomString;
}

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
void addUserToArray(char userName[10], int socketNumber, char password[40]) {
    printf("Client logged in as %s\n", userName);
    strcpy(users[userArrayIndex].userName, userName);
    users[userArrayIndex].clientSocketNo = socketNumber;
    strcpy(users[userArrayIndex].password,password);
=======
//FOR GETUSERS COMMAND
void getUsers(int socketNumber) {
    char *userString = malloc(100);
    if (users[0].userName != NULL) {
        strcpy(userString, users[0].userName);
        strcat(userString, ",");
    } else {
        return;
    }

    for (int i = 1; i < userArrayIndex; ++i) {
        strcat(userString, users[i].userName);
        strcat(userString, ",");
    }
    write(socketNumber, userString, strlen(userString) + 1);
}

//AFTER LOGIN COMMAND, ADD TO THE ARRAY
void addUserToArray(char userName[10], int socketNumber) {
    printf("Client logged in as %s\n", userName);
    strcpy(users[userArrayIndex].userName, userName);
    users[userArrayIndex].clientSocketNo = socketNumber;
>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd
    userArrayIndex++;
}

//LOGIN COMMAND
<<<<<<< HEAD
void loginUser(char userName[10], int socketNumber, char password[40]) {
    char *message = "login successful";
    write(socketNumber, message, strlen(message) + 1);
    addUserToArray(userName, socketNumber,password);
}

//SEND MESSAGE IF USER FOUND
void *sendMessage(void* context/*int clientSocketNumber, char *message*/) {
    struct Message *messageStruct = context;
    write(messageStruct->socketNo, messageStruct->message, strlen(messageStruct->message) + 1);
    //write(clientSocketNumber, message, strlen(message) + 1);
}

//SEND MESSAGE IF GROUP FOUND
=======
void loginUser(char userName[10], int socketNumber) {
    char *message = "login successful";
    write(socketNumber, message, strlen(message) + 1);
    addUserToArray(userName, socketNumber);
}

//SEND MESSAGE IF USER FOUND
void sendMessage(int clientSocketNumber, char *message) {
    write(clientSocketNumber, message, strlen(message) + 1);
}

>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd
void sendMessageToGroup(struct Group group, char *message) {
    for (int i = 0; i < group.groupSize; ++i) {
        write(group.person[i].clientSocketNo, message, strlen(message) + 1);
    }
}

<<<<<<< HEAD
//ADD GROUP TO ARRAY
=======
>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd
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

<<<<<<< HEAD
//CREATE GROUP AND CALL ADD-GROUP-TO-ARRAY FUNCTION
=======
>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd
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
<<<<<<< HEAD
    struct User null_user;
    strcpy(null_user.userName,"");
    return null_user;
}

//SEARCH GROUP FROM ARRAY
struct Group searchGroup(char groupName[20]) {
=======
}

struct Group searchGroup(char groupName[10]) {
>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd
    for (int i = 0; i < groupArrayIndex; ++i) {
        if (strcmp(group[i].groupName, groupName) == 0) {
            return group[i];
        }
    }
<<<<<<< HEAD
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
=======
}

>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd
int checkLogin(char userName[10]) {
    if (strcmp(searchUser(userName).userName, "") == 0) {
        return 0;
    } else
<<<<<<< HEAD
        return 1;
}

//CHECK IS THERE ANY GROUP WITH MULTIPLE NAME
=======
        return -1;
}

>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd
int checkMultiGroup(char groupName[20]) {
    if (strcmp(searchGroup(groupName).groupName, "") == 0) {
        return 0;
    } else
        return -1;
}
<<<<<<< HEAD

void *connection_handler(void *socket_desc) {
    //Get the socket descriptor
    pthread_t sendThread;
    int  sock             = *((int *) socket_desc);
    char receivedMessage[2000];  //client's message
    int  isLogged         = -1;
    int  readControl;
    int  parsedItemNumber = 0;

    //int new_sock = *((int *)sock);
=======
    
void *connection_handler(void *socket_desc) {
    //Get the socket descriptor
    char receivedMessage[2000];  //client's message

    int readControl;
    int parsedItemNumber = 0;
    int sock             = *((int *) socket_desc);

>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd
    while ((readControl = recv(sock, receivedMessage, 2000, 0)) > 0) {
        char *parsedCommand[50]; //parsedClientMessage

        parsedItemNumber = parsing(parsedCommand, receivedMessage, " ");

        if (strcmp(parsedCommand[0], "login") == 0 && parsedCommand[1] != NULL) {
<<<<<<< HEAD
            if (isLogged != 0)
                isLogged = checkLogin(parsedCommand[1]);

            else{
                struct Message message;
                message.socketNo = sock;
                strcpy(message.message,"Login First");
                pthread_create(&sendThread,NULL,sendMessage,&message);
                pthread_join(sendThread,NULL);
                continue;
            }

            if (isLogged != 1){
                char *password;
                password = createPassword(PASSWORD_LENGTH);
                loginUser(parsedCommand[1], sock,password);

                printf("Secret key generated for %s as %s\n",parsedCommand[1],password);

                char *sendPassword = malloc(100);

                strcpy(sendPassword,"password*");
                strcat(sendPassword,password);

                struct Message message;
                message.socketNo = sock;
                strcpy(message.message,sendPassword);
                pthread_create(&sendThread,NULL,sendMessage,&message);
                pthread_join(sendThread,NULL);
                free(sendPassword);
            }

            else{
                struct Message message;
                message.socketNo = sock;
                strcpy(message.message,"One login in one client");
                pthread_create(&sendThread,NULL,sendMessage,&message);
                pthread_join(sendThread,NULL);

            }

        } else if (isLogged != -1 & strcmp(parsedCommand[0], "getusers") == 0) {
            getUsers(sock);

        } else if (isLogged != -1 & strcmp(parsedCommand[0], "alias") == 0 && parsedCommand[1] != NULL &&
                   parsedCommand[2] != NULL) {
            int isCreatedBefore = checkMultiGroup(parsedCommand[1]);

            if (isCreatedBefore == 0) {
                createGroup(parsedCommand[1], parsedCommand[2], sock);
            } else {
                struct Message message;
                message.socketNo = sock;
                strcpy(message.message,"You can't create same group");
                pthread_create(&sendThread,NULL,sendMessage,&message);
                pthread_join(sendThread,NULL);

            }
        }
        else {
            if (isLogged == -1) {
                char *errorMessage = "You must login first";
                struct Message message;
                message.socketNo = sock;
                strcpy(message.message,errorMessage);
                pthread_create(&sendThread,NULL,sendMessage,&message);
                pthread_join(sendThread,NULL);

            }

            if (isLogged == 0 && parsedCommand[0] != NULL) {

                struct User  find = searchUser(parsedCommand[0]);
                struct Group findGroup = searchGroup(parsedCommand[0]);

                int control = 0;

=======
            int isLogged = checkLogin(parsedCommand[1]);
            if (isLogged == 0)
                loginUser(parsedCommand[1], sock);
            else {
                sendMessage(sock, "You can login only once");
            }

        } else if (strcmp(parsedCommand[0], "getusers") == 0) {
            getUsers(sock);

        } else if (strcmp(parsedCommand[0], "alias") == 0 && parsedCommand[1] != NULL && parsedCommand[2] != NULL) {
            int isCreatedBefore = checkMultiGroup(parsedCommand[1]);
            if (isCreatedBefore == 0) {
                createGroup(parsedCommand[1], parsedCommand[2], sock);
            } else {
                sendMessage(sock, "You can't create same group");
            }


        } else if (strcmp(parsedCommand[0], "exit") == 0) {
            close(sock);
            return 0;
        } else {
            if (parsedCommand[0] != NULL) {
                struct User  find      = searchUser(parsedCommand[0]);
                struct Group findGroup = searchGroup(parsedCommand[0]);

>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd
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
<<<<<<< HEAD
                    control = 1;

                    struct Message messageObject;
                    messageObject.socketNo = find.clientSocketNo;
                    strcpy(messageObject.message,message);
                    pthread_create(&sendThread,NULL,sendMessage,&messageObject);
                    pthread_join(sendThread,NULL);


                }
                if (strcmp(findGroup.groupName, "") != 0) {
=======
                    sendMessage(find.clientSocketNo, message);
                } else if (strcmp(findGroup.groupName, "") != 0) {

>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd
                    char *message = malloc(2000);
                    if (parsedCommand[1] != NULL) {
                        strcpy(message, parsedCommand[1]);
                        strcat(message, " ");
                    }
                    for (int i = 2; i < parsedItemNumber; ++i) {
                        strcat(message, parsedCommand[i]);
                        strcat(message, " ");
                    }
<<<<<<< HEAD
                    control = 1;

                    sendMessageToGroup(findGroup, message);
                }
                if(control == 0){
                    char *errorMessage = "There is no user or group with this name ";
                    struct Message messageObject;
                    messageObject.socketNo = sock;
                    strcpy(messageObject.message,errorMessage);
                    pthread_create(&sendThread,NULL,sendMessage,&messageObject);
                    pthread_join(sendThread,NULL);

=======

                    sendMessageToGroup(findGroup, message);
                } else {
                    perror("Your input was wrong");
>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd
                }
            }
        }
    }

    if (readControl == 0) {
        puts("Client disconnected");
<<<<<<< HEAD

        deleteUser(sock);
=======
>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd
        clientNumber--;
        fflush(stdout);
    } else if (readControl == -1) {
        perror("recv failed");
    }
<<<<<<< HEAD
    pthread_t send_thread, receive_thread;
=======
>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd

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

<<<<<<< HEAD


=======
>>>>>>> ca6cdab592a526ba74a53954cc0028eb277346cd
        puts("Client connected");
    }


    if (new_socket < 0) {
        perror("accept failed");
        return 1;
    }

    return 0;
}