#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#include "logger.h"

SSL_CTX* create_ssl_ctx(const char* cert_file, const char* key_file) {
    const SSL_METHOD* method = TLS_server_method();  // TLS_server_method for OpenSSL 3.0
    SSL_CTX* ctx = SSL_CTX_new(method);

    if (!ctx) {
        log_error("Unable to create SSL context");
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM) <= 0) {
        log_error("Unable to load certificate");
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM) <= 0) {
        log_error("Unable to load private key");
        exit(EXIT_FAILURE);
    }

    if (!SSL_CTX_check_private_key(ctx)) {
        log_error("Private key does not match certificate");
        exit(EXIT_FAILURE);
    }

    return ctx;
}
