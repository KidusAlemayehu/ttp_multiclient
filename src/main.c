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
#include "connection.h"
#include "config.h"
#include "logger.h"


#define PORT 8050

typedef struct{
    int client_fd;
    SSL *ssl;
} client_info_t;

void* client_handler    (void* arg){
    client_info_t *client_info = (client_info_t*) arg;
    int client_fd = client_info->client_fd;
    SSL *ssl = client_info->ssl;

    log_info("Handling new SSL connection ...");
    handle_connection(ssl, client_fd);

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