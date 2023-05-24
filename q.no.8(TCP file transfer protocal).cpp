#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket, read_size;
    struct sockaddr_in server_address, client_address;
    char buffer[BUFFER_SIZE];

    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Failed to create socket\n");
        return 1;
    }

    // Set up server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind the socket to the specified IP and port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        return 1;
    }

    // Listen for incoming connections
    listen(server_socket, 3);
    printf("Waiting for incoming connections...\n");

    // Accept an incoming connection
    int client_address_size = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t *)&client_address_size);
    if (client_socket < 0) {
        perror("Accept failed");
        return 1;
    }
    printf("Client connected\n");

    // Receive file from client
    FILE *received_file = fopen("received_file.txt", "w");
    if (received_file == NULL) {
        perror("Error opening file");
        return 1;
    }

    while ((read_size = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, read_size, received_file);
    }

    if (read_size < 0) {
        perror("Receive failed");
        return 1;
    }

    printf("File received and saved as 'received_file.txt'\n");

    // Close the sockets and file
    fclose(received_file);
    close(client_socket);
    close(server_socket);

    return 0;
}
