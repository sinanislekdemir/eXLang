#ifndef EXLANG_CMD_DIGITAL_HPP
#define EXLANG_CMD_DIGITAL_HPP

#include "exlang/program.hpp"

namespace exlang {
#ifdef MICRO_DEVICE
int command_pinmode(command c, program *p);
int command_digitalwrite(command c, program *p);
int command_digitalread(command c, program *p);
#endif
}; // namespace exlang

#endif
