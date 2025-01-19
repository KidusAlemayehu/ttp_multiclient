#ifndef CONNECTION_H
#define CONNECTION_H

#include <openssl/ssl.h>

void handle_connection(SSL *ssl, int client_fd);

#endif