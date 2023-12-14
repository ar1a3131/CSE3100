#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/select.h>

#define MAX_CLIENTS 10
#define PORT 8026

int client_sockets[MAX_CLIENTS] = {0};

void send_message(int sender, char *message) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] > 0 && client_sockets[i] != sender) {
            send(client_sockets[i], message, strlen(message), 0);
        }
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    fd_set readfds;
    int max_socket;

    // Create a socket for the server
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the server socket to the specified address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_socket = server_fd;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            int socket = client_sockets[i];
            if (socket > 0) {
                FD_SET(socket, &readfds);
                if (socket > max_socket) {
                    max_socket = socket;
                }
            }
        }

        // Use select() to monitor multiple sockets for activity
        int activity = select(max_socket + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("Select error");
            exit(EXIT_FAILURE);
        }

        // Handle new client connections
        if (FD_ISSET(server_fd, &readfds)) {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                perror("Accept failed");
                exit(EXIT_FAILURE);
            }

            int i;
            for (i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;
                    break;
                }
            }
        }

        // Handle client messages
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int client_socket = client_sockets[i];
            if (FD_ISSET(client_socket, &readfds)) {
                char client_message[1024];
                memset(client_message, 0, sizeof(client_message));
                int valread = recv(client_socket, client_message, sizeof(client_message), 0);
                if (valread <= 0) {
                    // Client disconnected or encountered an error
                    close(client_socket);
                    client_sockets[i] = 0;
                } else {
                    if (strncmp(client_message, "$exit\n", 6) == 0) {
                        // Client wants to exit
                        send_message(client_socket, "User has left the chat.\n");
                        close(client_socket);
                        client_sockets[i] = 0;
                    } else {
                        send_message(client_socket, client_message);
                    }
                    //printf("Client%d: %s", i, client_message);
                }
            }
        }
    }

    return 0;
}
