#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void error(const char *message) {
    perror(message);
    exit(1);
}

int main() {
    int client_fd, valread;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        error("Socket creation failed");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr)) <= 0) {
        error("Invalid address/ Address not supported");
    }

    // Connect to server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("Connection failed");
    }

    // Read file to send
    FILE *file = fopen("file_to_send.txt", "r");
    if (file == NULL) {
        error("File open failed");
    }

    // Send
