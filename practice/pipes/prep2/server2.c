#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <errno.h>
#include <sys/time.h>

#define MAXCLIENTS 10
#define PORT 8081
#define BUFFER_SIZE 1024

int clientSocket[MAXCLIENTS] = {0};

int checkError(int status) {
    if (status < 0) {
        perror("Socket Error");
        return 1;
    }
    return 0;
}

void handleClientConversation(int currentClientSocket);
void sendMessage(int sender, char *message);

void sendMessage(int sender, char *message) {
    for (int i = 0; i < MAXCLIENTS; i++) {
        if (clientSocket[i] > 0 && clientSocket[i] != sender) {
            send(clientSocket[i], message, strlen(message), 0);
        }
    }
}

int main(int argc, char *argv[]) {
    int status;
    int newClientSocket;

// Socket structure - server*********************************
    int sid = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    status = bind(sid, (struct sockaddr *)&addr, sizeof(addr));
    checkError(status);
//**************************************************************


    status = listen(sid, MAXCLIENTS);
    checkError(status);

    printf("Server listening on port:%d...\n", PORT);

    fd_set afds;
    int maxSocket;
    while (1) {
        FD_ZERO(&afds);
        FD_SET(sid, &afds);

        maxSocket = sid;

        for (int i = 0; i < MAXCLIENTS; i++) {
            int sockets = clientSocket[i];
            if (sockets > 0) {
                FD_SET(sockets, &afds);
                if (sockets > maxSocket) {
                    maxSocket = sockets;
                }
            }
        }

        int activity = select(maxSocket + 1, &afds, NULL, NULL, NULL);
        checkError(activity);

        if (FD_ISSET(sid, &afds)) {
            newClientSocket = accept(sid, (struct sockaddr *)&addr, &addrlen);

            if (newClientSocket < 0) {
                perror("Accept failed");
                exit(EXIT_FAILURE);
            }

            for (int i = 0; i < MAXCLIENTS; i++) {
                if (clientSocket[i] == 0) {
                    clientSocket[i] = newClientSocket;
                    break;
                }
            }
        }

        for (int i = 0; i < MAXCLIENTS; i++) {
            int currentClientSocket = clientSocket[i];
            if (FD_ISSET(currentClientSocket, &afds)) {
                char clientMessage[BUFFER_SIZE];
                memset(clientMessage, 0, sizeof(clientMessage));

                int dataRead = recv(currentClientSocket, clientMessage, sizeof(clientMessage), 0);

                if (dataRead <= 0) {
                    close(currentClientSocket);
                    clientSocket[i] = 0;
                } else {
                    if (strncmp(clientMessage, "EXIT\n", 6) == 0) {
                        sendMessage(currentClientSocket, "User has left the chat\n");
                        close(currentClientSocket);
                        clientSocket[i] = 0;
                    } else {
                        sendMessage(currentClientSocket, clientMessage);
                    }
                }
            }
        }
    }
    return 0;
}





