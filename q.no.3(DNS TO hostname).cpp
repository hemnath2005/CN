#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define DNS_SERVER "8.8.8.8"  // Example DNS server (Google DNS)
#define DNS_PORT 53           // DNS server port

// DNS header structure
struct DNSHeader {
    unsigned short id;
    unsigned short flags;
    unsigned short qdcount;
    unsigned short ancount;
    unsigned short nscount;
    unsigned short arcount;
};

// DNS question structure
struct DNSQuestion {
    unsigned short qtype;
    unsigned short qclass;
};

// DNS response structure
struct DNSResponse {
    unsigned short name;
    unsigned short type;
    unsigned short dnsclass;
    unsigned int ttl;
    unsigned short rdlength;
    unsigned char rdata[4];
};

// Function to create a DNS query packet
void create_dns_query_packet(char *hostname, unsigned char *buf) {
    struct DNSHeader *dns = (struct DNSHeader *)buf;
    struct DNSQuestion *qinfo = (struct DNSQuestion *)(buf + sizeof(struct DNSHeader));
    
    // Set DNS header
    dns->id = (unsigned short)htons(getpid());
    dns->flags = htons(0x0100);  // Standard query
    dns->qdcount = htons(1);
    dns->ancount = 0;
    dns->nscount = 0;
    dns->arcount = 0;
    
    // Set DNS question
    qinfo->qtype = htons(1);  // Type A (IPv4 address)
    qinfo->qclass = htons(1); // Class IN (Internet)
    
    // Convert hostname to DNS format
    char *token;
    int index = 0;
    token = strtok(hostname, ".");
    
    while (token != NULL) {
        buf[index++] = strlen(token);
        memcpy(&buf[index], token, strlen(token));
        index += strlen(token);
        token = strtok(NULL, ".");
    }
    
    buf[index++] = 0; // End of hostname
    buf[index++] = 0x01; // Type A
    buf[index++] = 0x00; // Class IN
}

int main() {
    char hostname[100];
    printf("Enter the hostname to resolve: ");
    scanf("%s", hostname);
    
    // Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }
    
    // Set DNS server address and port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(DNS_PORT);
    if (inet_pton(AF_INET, DNS_SERVER, &(server_addr.sin_addr)) <= 0) {
        perror("Invalid DNS server address");
        return 1;
    }
    
    // Create DNS query packet
    unsigned char buf[65536];
    create_dns_query_packet(hostname, buf);
    
    // Send DNS query packet to server
    if (sendto(sockfd, buf, sizeof(struct DNSHeader) + sizeof(struct DNSQuestion) + strlen(hostname) + 2, 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("DNS query failed");
        return 1;
    }
    
    // Receive DNS response
    unsigned int server_addr_len = sizeof(server_addr
    
