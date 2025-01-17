#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include "socket_utils.h"
#include "ssl_utils.c"
#include "rate_limiter.c"

#define PORT 8050

typedef struct{
    int client_fd;
    SSL *ssl;
} client_info_t;

void* client_handler    (void* arg){
    client_info_t *client_info = (client_info_t*) arg;
    int client_fd = client_info->client_fd;
    SSL *ssl = client_info->ssl;

    rate_limiter_t limiter = {time(NULL), 0};

    char buffer[1024];
    int bytes;

    while ((bytes = SSL_read(ssl, buffer, sizeof(buffer))) > 0){
        if(!check_rate_limit(&limiter)){
            printf("Rate Limit exceeded ...\n");
            break;
        }
        printf("Received data: %s\n", buffer);

        if (SSL_write(ssl, buffer, bytes) <= 0) {
            perror("SSL_write failed");
            break;
        }
    }

    SSL_free(ssl);
    close(client_fd);
    free(client_info);
    return NULL;
}

int main(){
    int server_fd, client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    OPENSSL_init_ssl(0, NULL);
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG, NULL);  

    SSL_CTX *ctx = create_ssl_ctx("server.crt", "server.key");
    
    server_fd = create_socket(PORT);
    printf("Server listening on port 8050\n");
    while(1){
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0){
            perror("connection accept failure");
            continue;
        }

        SSL *ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_fd);
        if (SSL_accept(ssl) <= 0){
            perror("SSL_accept failure");
            SSL_free(ssl);
            close(client_fd);
            continue;
        }

        client_info_t *client_info = (client_info_t*) malloc(sizeof(client_info_t));
        client_info->client_fd = client_fd;
        client_info->ssl = ssl;

        pthread_t thread;
        if(pthread_create(&thread, NULL, client_handler, client_info) != 0){
            perror("pthread_create failure");
            SSL_free(ssl);
            close(client_fd);
            free(client_info);
        }
        printf("Connection accepted\n");
    }
    close(server_fd);
    SSL_CTX_free(ctx);
    return 0;
}