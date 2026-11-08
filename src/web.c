#include "web.h"

static int sockfd;

// create socket, address, than bind and listen
void web_init(char *IPv, int port) {
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
}

void web_loop(void) {
	int client = accept(sockfd, NULL, NULL);
	int client_process = fork();
	if (!client_process) {
		close(sockfd);
		char buffer[2048];
		read(client, buffer, 2047);
		char *response = "HTTP/1.1 200 OK\r\nContent-Type: plain/text; charset=UTF-8\r\nContent-Length: 7\r\n\r\nHello!\n";
		write(client, response, strlen(response));
		close(client);
		_exit(0);
	}
	close(client);
}
