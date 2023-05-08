#ifndef EXLANG_CMD_INTERRUPTS_HPP
#define EXLANG_CMD_INTERRUPTS_HPP

#include "exlang/program.hpp"

namespace exlang {
#ifdef MICRO_DEVICE
int command_int(command c, program *p);
#endif
}; // namespace exlang
#endif
