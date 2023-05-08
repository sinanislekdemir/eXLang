#ifndef EXLANG_CMD_LOGIC_HPP
#define EXLANG_CMD_LOGIC_HPP

#include "exlang/program.hpp"

namespace exlang {
int command_je(command c, program *p);	// Jump if a = b
int command_jne(command c, program *p); // Jump if a ! b
int command_jg(command c, program *p);	// Jump if a > b
int command_jge(command c, program *p); // Jump if a >= b
int command_jl(command c, program *p);	// Jump if a < b
int command_jle(command c, program *p); // Jump if a <= b

int command_cmp(command c, program *p);

int command_lrotate(command c, program *p);
int command_rrotate(command c, program *p);
int command_lshift(command c, program *p);
int command_rshift(command c, program *p);
}; // namespace exlang
#endif
