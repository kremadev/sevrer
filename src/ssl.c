#include "ssl.h"
#include <stdio.h>
#include <stdlib.h>

static SSL_CTX *ctx;

void ssl_init(char *cert, char *key) {
	ctx = SSL_CTX_new(TLS_server_method());
	if (!ctx
	    || SSL_CTX_use_certificate_file(ctx, cert, SSL_FILETYPE_PEM) != 1
	    || SSL_CTX_use_PrivateKey_file(ctx, key, SSL_FILETYPE_PEM) != 1) {
		ERR_print_errors_fp(stderr);
		fprintf(stderr, "failed to load certificate/key\n");
		exit(1);
	}
}

// returns NULL if the handshake fails
SSL *ssl_client(int client_fd) {
	SSL *ssl = SSL_new(ctx);
	if (!ssl)
		return NULL;
	SSL_set_fd(ssl, client_fd);
	if (SSL_accept(ssl) != 1) {
		SSL_free(ssl);
		return NULL;
	}
	return ssl;
}

void ssl_close(SSL *ssl) {
	SSL_shutdown(ssl);
	SSL_free(ssl);
}
