#ifndef SSL_H
#define SSL_H

#include <openssl/ssl.h>
#include <openssl/err.h>

#define ssl_read SSL_read
#define ssl_write SSL_write
#define ssl_shutdown SSL_shutdown
#define ssl_free SSL_free

void ssl_init(char *cert, char *key);
SSL *ssl_client(int client_fd);
void ssl_close(SSL *ssl);

#endif
