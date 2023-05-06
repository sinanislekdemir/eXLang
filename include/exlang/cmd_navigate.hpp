#ifndef EXLANG_CMD_NAVIGATE_HPP
#define EXLANG_CMD_NAVIGATE_HPP

#include "exlang/program.hpp"

int command_call(command c, program *p);
int command_goto(command c, program *p);
int command_halt(command c, program *p);
#endif
