#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include <math.h>   

#include "addition.h"
#include "subtraction.h"
#include "multiplication.h"
#include "division.h"

#define PORT 65432
#define BUFFER_SIZE 1024

// Function to handle client requests
void* handle_client(void* socket) {
    int new_socket = *((int*)socket);
    char buffer[BUFFER_SIZE] = {0};

    // Main loop to handle client requests
    while(1) {
        // Receive arithmetic expression from the client
        int valread = recv(new_socket, buffer, BUFFER_SIZE, 0);
        if (valread == -1 || valread == 0) {
            printf("Connection closed by client\n");
            break;
        }
        buffer[valread] = '\0';

        // Parse the arithmetic expression
        double num1, num2;
        char operator;
        sscanf(buffer, "%lf %c %lf", &num1, &operator, &num2);

        // Calculate the result
        double result;
        switch(operator) {
            case '+':
                result = add(num1, num2);
                break;
            case '-':
                result = subtract(num1, num2);
                break;
            case '*':
                result = multiply(num1, num2);
                break;
            case '/':
                result = divide(num1, num2);
                break;
            default:
                result = NAN; // Indicates invalid operator
        }

        // Convert the result to a string
        char result_str[BUFFER_SIZE];
        snprintf(result_str, BUFFER_SIZE, "%.2f", result);

        // Send the result back to the client
        send(new_socket, result_str, strlen(result_str), 0);
    }

    // Close the socket
    close(new_socket);
    free(socket);
    pthread_exit(NULL);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) == -1) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    // Accept and handle incoming connections in a loop
    while(1) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);

        // Accept a connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len)) == -1) {
            perror("accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        printf("Connected to client\n");

        // Create a separate thread to handle the client
        pthread_t thread_id;
        int* new_sock = (int*)malloc(sizeof(int));
        *new_sock = new_socket;
        if (pthread_create(&thread_id, NULL, handle_client, (void*)new_sock) != 0) {
            perror("pthread_create failed");
            close(new_socket);
            free(new_sock);
        }
        pthread_detach(thread_id);  // To automatically free resources when thread finishes
    }

    // Close the server socket (this part of the code is never reached)
    close(server_fd);

    return 0;
}
