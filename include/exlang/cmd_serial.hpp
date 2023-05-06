#ifndef EXLANG_CMD_SERIAL_HPP
#define EXLANG_CMD_SERIAL_HPP

#include "exlang/program.hpp"

int command_serial_println(command c, program *p);
int command_serial_print(command c, program *p);
int command_getln(command c, program *p);

#endif
