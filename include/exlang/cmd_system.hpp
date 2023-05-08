#ifndef EXLANG_CMD_SYSTEM_HPP
#define EXLANG_CMD_SYSTEM_HPP

#include "exlang/program.hpp"

namespace exlang {
typedef int (*syscall_function)(program *_p);

struct sys_call {
	int (*fnc)(program *_p);
	int driver_address;
	sys_call *next;
};

int command_core(command c, program *p);
int command_sys(command c, program *p);
int command_noop(command c, program *p);
}; // namespace exlang

#endif
