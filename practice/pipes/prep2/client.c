#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8026

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ip_address>\n", argv[0]);
        exit(-1);
    }

    int client_socket;
    struct sockaddr_in server_addr;
    char message[1024];

    // Create a socket for the client
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("error during socket creation");
        exit(-1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
        perror("invalid address");
        exit(-1);
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connection failed");
        exit(-1);
    }

    printf("Connected to the chat room. Type your messages (press Ctrl+C to exit).\n");

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process: receive and display messages from the server
        while (1) {
            memset(message, 0, sizeof(message));
            int valread = recv(client_socket, message, sizeof(message), 0);
            if (valread <= 0) {
                // Server closed the connection
                break;
            }
            printf(message);
        }
    } else {
        // Parent process: send messages to the server
        while (1) {
            printf(" ");
            fgets(message, sizeof(message), stdin);
            send(client_socket, message, strlen(message), 0);
            if (strncmp(message, "$exit\n", 6) == 0) {
            printf("You have left the chat.\n");
            break;
        }
        }
    }

    close(client_socket);

    return 0;
}
