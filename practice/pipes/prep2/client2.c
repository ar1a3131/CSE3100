#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define PORT 8081

void checkError(int status, int line) {
    if (status < 0) {
        perror("Socket Error");
        exit(-1);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <hostname>\n", argv[0]);
        return 1;
    }

// Socket structure******************************************
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct hostent *server = gethostbyname(argv[1]);
    if (server == NULL) {
        printf("Couldn't find a host named: %s\n", argv[1]);
        return 2;
    }

    struct sockaddr_in srv;
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    memcpy(&srv.sin_addr, server->h_addr_list[0], server->h_length);

    int status = connect(clientSocket, (struct sockaddr *)&srv, sizeof(srv));
    checkError(status, __LINE__);
//************************************************************




    printf("Connected to the chat room! Type your messages ($exit to exit the chat room):\n");

    pid_t pid = fork();
    char message[BUFFER_SIZE];

    if (pid == 0) { // Child process
        while (1) {
            memset(message, 0, sizeof(message));
            int dataRead = recv(clientSocket, message, sizeof(message), 0);
            if (dataRead <= 0) {
                break; // End connection
            }
            printf("New message: %s", message);
        }
    } else { // Parent process
        while (1) {
            printf("Enter a message: ");
            fgets(message, BUFFER_SIZE, stdin);

            send(clientSocket, message, strlen(message), 0);

            if (strncmp(message, "$exit\n", 6) == 0) {
                printf("You have left the chat.\n");
                break;
            }
        }
    }

    close(clientSocket);

    return 0;
}