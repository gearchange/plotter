#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "gearcode.h"
#include "inset.h"

int (*strncmp_icase)(char const *, char const *, size_t) = strncmp;

typedef struct {
	char const * const op_str;
	int (*commandfunc)(int, char[16][16]);
	enum {
#define inst(s, f) s,
#include "inset.h"
#undef inst
	} opecode;
} command;

command cmd[] = {
#define inst(s, f) {#s, f},
#include "inset.h"
#undef inst
};

size_t cmd_length = sizeof(cmd) / sizeof(cmd[0]);

int do_command(char str[STRSIZE]) {
	strchr(str, '#')?*strchr(str, '#') = '\0':0;
	strstr(str, "//")?*strstr(str, "//") = '\0':0;
	int argc = 0;
	char argv[16][16] = {""};
	char *com = strtok(str, "\t1 ");
	if (!com) return 000;
	for (argc = 0; com; argc++) {
		strncpy(argv[argc], com, 16);
		com = strtok(NULL, "\t ");
	}
	for (int i = 0; i < cmd_length; i++) if (!strncmp_icase(argv[0], cmd[i].op_str, 16)) return cmd[i].commandfunc(argc, argv);
	return 0;
}

