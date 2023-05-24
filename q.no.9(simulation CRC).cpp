#include <stdio.h>
#include <string.h>

#define GENERATOR "1001" // CRC-4 (4-bit generator polynomial)

void generateCRC(char *message, char *generator, char *crc) {
    int msg_len = strlen(message);
    int gen_len = strlen(generator);
    int crc_len = gen_len - 1;

    strcpy(crc, message); // Copy message to crc initially

    // Perform modulo-2 division
    for (int i = 0; i <= msg_len - crc_len; i++) {
        if (crc[i] == '1') {
            for (int j = 0; j < gen_len; j++) {
                crc[i + j] = crc[i + j] == generator[j] ? '0' : '1';
            }
        }
    }
}

int verifyCRC(char *message, char *generator, char *received_crc) {
    char crc[100];
    generateCRC(message, generator, crc);

    // Check if the generated CRC matches the received CRC
    if (strcmp(crc, received_crc) == 0) {
        return 1; // No error detected
    } else {
        return 0; // Error detected
    }
}

int main() {
    char message[100];
    char received_crc[10];

    printf("Enter the message: ");
    scanf("%s", message);

    printf("Enter the received CRC: ");
    scanf("%s", received_crc);

    int isErrorFree = verifyCRC(message, GENERATOR, received_crc);

    if (isErrorFree) {
        printf("Received message is error-free.\n");
    } else {
        printf("Error detected in the received message.\n");
    }

    return 0;
}
