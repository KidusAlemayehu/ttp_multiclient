#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define SERVER_PORT 8050
#define SERVER_IP "127.0.0.1"

int main() {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        perror("Unable to create SSL context");
        exit(1);
    }

    SSL *ssl = SSL_new(ctx);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Unable to create socket");
        exit(1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Unable to connect to server");
        exit(1);
    }

    SSL_set_fd(ssl, server_fd);
    if (SSL_connect(ssl) <= 0) {
        perror("SSL connect failed");
        exit(1);
    }

    char *message = "Hello, Server!";
    SSL_write(ssl, message, strlen(message));

    char buffer[1024];
    int bytes_read;

    while (1) {
        bytes_read = SSL_read(ssl, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            if (bytes_read == 0) {
                printf("Server closed the connection\n");
            } else {
                perror("SSL_read failed");
            }
            break;
        }

        buffer[bytes_read] = '\0';
        printf("Received data from server: %s\n", buffer);

        const char *response = "Data received successfully!";
        SSL_write(ssl, response, strlen(response));
    }

    SSL_free(ssl);
    close(server_fd);
    SSL_CTX_free(ctx);
    return 0;
}
