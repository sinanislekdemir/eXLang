#ifndef EXLANG_CMD_LOG_HPP
#define EXLANG_CMD_LOG_HPP

#include "exlang/program.hpp"

namespace exlang {
void set_output(int output_type);
int command_log(command c, program *p);
}; // namespace exlang

#endif
