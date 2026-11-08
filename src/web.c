#include "web.h"
#include "ssl.h"
#include "file.h"

static int sockfd;

// create socket, address, than bind and listen
void web_init(char *IPv, int port, char *cert, char *key) {
	int opt = 1;
	if (!IPv) {
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 1) {
			perror("socket");
			exit(1);
		}
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);
		struct sockaddr_in addr = {
			.sin_family = AF_INET,
			.sin_port = htons(port),
		};
		if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr))) {
			perror("bind");
			exit(1);
		}
	} else {
		sockfd = socket(AF_INET6, SOCK_STREAM, 0);
		if (sockfd < 1) {
			perror("socket");
			exit(1);
		}
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);
		struct sockaddr_in6 addr = {
			.sin6_family = AF_INET6,
			.sin6_port = htons(port),
		};
		if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr))) {
			perror("bind");
			exit(1);
		}
	}
	if (listen(sockfd, 10)) {
		perror("listen");
		exit(1);
	}
	signal(SIGCHLD, SIG_IGN);
	ssl_init(cert, key);
}

void web_loop(void) {
	int client = accept(sockfd, NULL, NULL);
	if (client < 0) {
		perror("accept");
		return;
	}
	int client_process = fork();
	if (client_process == 0) {
		close(sockfd);
		SSL *ssl = ssl_client(client);
		if (ssl) {
			char buffer[4096];
			int n = ssl_read(ssl, buffer, sizeof(buffer) - 1);
			if (n > 0) {
				buffer[n] = '\0';
				int size = 0;
				char *page = NULL;
				char *end_of_path = NULL;
				if (!strncmp(buffer, "GET ", 4))
					end_of_path = strchr(buffer + 4, ' ');
				if (end_of_path) {
					*end_of_path = '\0';
					page = get_file_content(buffer + 4, &size);
				} else {
					const char *bad = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
					ssl_write(ssl, bad, strlen(bad));
				}
				if (page) {
					ssl_write(ssl, page, size);
					free(page);
				}
			}
			ssl_close(ssl);
		}
		close(client);
		_exit(0);
	}
	close(client);
}
