#ifndef EXLANG_COMMAND_HPP
#define EXLANG_COMMAND_HPP

namespace exlang {
static constexpr int VAR_TYPE_NONE = 0;
static constexpr int VAR_TYPE_NUM = 1;
static constexpr int VAR_TYPE_STR = 2;

#ifndef MAX_CMDS
#define MAX_CMDS 100 // 1000 for ESP32 // NOLINT
#endif

#include "exlang/memory.hpp"

struct command {
	char statement; // no need to copy the cmd
	int variable_index[3];
	short variable_type[3];
	short arg_count;
	char pid;
	bool exception;
};

double get_double(command c, int index);
long get_long(command c, int index);
void get_string(command c, int index, char *back, unsigned int size);
char get_byte(command c, int index);
}; // namespace exlang

#endif
