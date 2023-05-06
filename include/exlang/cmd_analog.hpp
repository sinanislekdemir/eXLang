#ifndef EXLANG_CMD_ANALOG_HPP
#define EXLANG_CMD_ANALOG_HPP

#include "exlang/program.hpp"

#ifdef MICRO_DEVICE
int command_analogread(command c, program *p);
int command_analogwrite(command c, program *p);
int command_analogref(command c, program *p);
#endif

#endif
