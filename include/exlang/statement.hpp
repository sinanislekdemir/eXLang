#ifndef EXLANG_STATEMENT_HPP
#define EXLANG_STATEMENT_HPP
#define MAX_COMMAND_LENGTH 10
#define NUM_STATEMENTS 64

#include "exlang/constants.hpp"
#include "exlang/program.hpp"

typedef int (*statement_function)(command c, program *p);

struct statement {
	int (*f)(command c, program *p);
	char command;
};

void register_statements();
char find_statement(const char *cmd);

#endif
