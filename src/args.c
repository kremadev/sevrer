#include "args.h"

#define MAX_ARGS 64

static arg_t arguments[MAX_ARGS];
static int argument_count = 0;

void args_add_argument(char *shortv, char *longv, char **dest, int takes_value) {
	arguments[argument_count].shortv = shortv;
	arguments[argument_count].longv = longv;
	arguments[argument_count].dest = dest;
	arguments[argument_count].flag = takes_value;
	if (takes_value == 0) *dest = 0;
	argument_count++;
}

void args_check_arguments(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Not enough arguments.\n");
		exit(1);
	} else {
		for (int i = 1; i < argc; i++) {
			int found = 0;
			for (int j = 0; j < argument_count; j++) {
				if (!strcmp(argv[i], arguments[j].shortv) || !strcmp(argv[i], arguments[j].longv)) {
					if (!arguments[j].flag) {
						*arguments[j].dest = argv[i];
						found = 1;
						break;
					} else if (i + 1 < argc) {
						*arguments[j].dest = argv[i+1];
						i++;
						found = 1;
						break;
					} else {
						fprintf(stderr, "%s needs a value.\n", argv[i]);
						exit(1);
					}
				}
			}
			if (!found) {
				fprintf(stderr, "Unknown argument %s.\n", argv[i]);
				exit(1);
			}
		}
	}
}
