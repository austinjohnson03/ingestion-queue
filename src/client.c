// Test client for server

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 5100
#define BUFFER_SIZE 1024

int client(void) {
    struct sockaddr_in serv_addr;
    char const *hello = "Hello from client.\n";

    int const client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd < 0) {
        fprintf(stderr, "Error creating client socket.\n");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);


    int const av = inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    if (av < 0) {
        fprintf(stderr, "Invalid address value.\n");
        exit(1);
    }

    int const status = connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (status < 0) {
        fprintf(stderr, "Error connecting to server.\n");
        exit(1);
    }

    send(client_fd, hello, strlen(hello), 0);
    printf("Message sent from client.\n");

    close(client_fd);

    return 0;
}