#include "exlang/interpreter.hpp"
#include "exlang/statement.hpp"

extern statement statements[NUM_STATEMENTS];
extern command commands[MAX_CMDS];

int run(unsigned short c, program *_p) {
	for (unsigned int i = 0; i < NUM_STATEMENTS; i++) {
		if (commands[c].statement == statements[i].command) {
			return statements[i].f(commands[c], _p);
		}
	}

#ifndef DISABLE_EXCEPTIONS
	commands[c].exception = true;
	error_msg("Unknown command", _p->pid);
#endif

	return -1;
}
