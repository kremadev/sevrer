#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "args.h"
#include "web.h"

// for arguments
#define ARG_VALUE 1
#define ARG_BOOL 0

enum {
	ARG_TRASH,
	ARG_HELP,
	ARG_IPV,
	ARG_PORT,
	ARG_DIR,
};

int main(int argc, char **argv) {
	char *args[64] = {0};
	int port = 8080;
	args_add_argument("-h", "--help", &args[ARG_HELP], ARG_BOOL);
	args_add_argument("-4", "--ipv4", &args[ARG_TRASH], ARG_BOOL);
	args_add_argument("-6", "--ipv6", &args[ARG_IPV], ARG_BOOL);
	args_add_argument("-p", "--port", &args[ARG_PORT], ARG_VALUE);
	args_add_argument("-d", "--directory", &args[ARG_DIR], ARG_VALUE);
	args_check_arguments(argc, argv);

	if (args[ARG_PORT])
		port = atoi(args[ARG_PORT]);
	if (args[ARG_HELP]) {
		printf("Usage:\n"
		       "\t-6/4 --ipv6/4 # IPv6 and IPv4\n"
		       "\t-p --port [PORT] # for custom port\n"
		       "\t-h --help # prints this menu\n"
		       "\t-d --directory [DIRECTORY] # gets contents from directory\n");
		exit(0);
	}
	printf("Using port: %d\n", port);

	

	return 0;
}
