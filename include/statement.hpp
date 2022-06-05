#ifndef _statement_hpp
#define _statement_hpp
#define MAX_COMMAND_LENGTH 10
#ifdef MICRO_DEVICE
#define NUM_STATEMENTS 56
#else
#define NUM_STATEMENTS 50
#endif

#include "constants.hpp"
#include "program.hpp"

typedef int (*statement_function)(command c, program *p);

struct statement {
	int (*f)(command c, program *p);
	char command;
};

void register_statements();
char find_statement(const char *cmd);

#endif
