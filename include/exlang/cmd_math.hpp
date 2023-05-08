#ifndef EXLANG_CMD_MATH_HPP
#define EXLANG_CMD_MATH_HPP

#include "exlang/program.hpp"

namespace exlang {
int command_inc(command c, program *p);
int command_num(command c, program *p);
int command_rand(command c, program *p);
int command_add(command c, program *p);
int command_sub(command c, program *p);
int command_div(command c, program *p);
int command_mul(command c, program *p);
int command_xor(command c, program *p);
int command_or(command c, program *p);
int command_and(command c, program *p);
int command_pow(command c, program *p);
int command_trigonometry(command c, program *p);
}; // namespace exlang
#endif
