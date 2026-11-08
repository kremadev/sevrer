#ifndef WEB_H
#define WEB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void web_init(char *IPv, int port, char *cert, char *key);
void web_loop(void);

#endif
