#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 53
#define MAX_BUFFER_SIZE 1024

// Function to perform DNS lookup and return the IP address
char* dns_lookup(char* hostname) {
    struct hostent* host;
    struct in_addr **addr_list;
    int i;

    if ((host = gethostbyname(hostname)) == NULL) {
        herror("gethostbyname");
        return NULL;
    }

    addr_list = (struct in_addr **)host->h_addr_list;

    for (i = 0; addr_list[i] != NULL; i++) {
        return inet_ntoa(*addr_list[i]);
    }

    return NULL;
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[MAX_BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("DNS server is listening on port %d...\n", PORT);

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        // Receive DNS request from the client
        ssize_t len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);
        buffer[len] = '\0';

        printf("Received DNS request: %s\n", buffer);

        // Perform DNS lookup
        char* ip_address = dns_lookup(buffer);

        if (ip_address != NULL) {
            printf("DNS lookup successful. IP address: %s\n", ip_address);
            // Send the IP address back to the client
            sendto(sockfd, ip_address, strlen(ip_address), 0, (struct sockaddr *)&client_addr, addr_len);
        } else {
            printf("DNS lookup failed for %s\n", buffer);
            // Send an error message to the client
            char* error_message = "DNS lookup failed";
            sendto(sockfd, error_message, strlen(error_message), 0, (struct sockaddr *)&client_addr, addr_len);
        }
    }

    return 0;
}
