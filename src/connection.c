#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "rate_limiter.c"
#include "connection.h"
#include "logger.h"

void handle_connection(SSL *ssl, int client_fd){
    char buffer[1024];
    int bytes_count;
    char ip_addr[16];

    get_client_ip_addr(client_fd, ip_addr);

    while (1)
    {
        bytes_count = SSL_read(ssl, buffer, sizeof(buffer));
        if (bytes_count <= 0) {
            if (bytes_count == 0) {
                printf("Client disconnected\n");
            } else {
                perror("SSL_read failed");
            }
            break;
        }
        if(!check_rate_limit(ip_addr)){
            printf("Rate Limit exceeded ...\n");
            break;
        }

        buffer[bytes_count] = '\0';
        printf("Received data: %s\n", buffer);
        const char *response = "Data received successfully"; 
        SSL_write(ssl, response, strlen(response));
    }

    SSL_free(ssl);
    close(client_fd);
}