#ifndef EXLANG_CMD_VARIABLE_HPP
#define EXLANG_CMD_VARIABLE_HPP

#include "exlang/program.hpp"

// Create variable SET <var> <val>
int command_set(command c, program *p);
// Copy variable CPY <dest> <from> <size> <src>
int command_cpy(command c, program *p);
// Delete variable DEL <var>
int command_del(command c, program *p);
// Convert variable (number) into string STR <dest> <src>
int command_str(command c, program *p);
// Convert varible (string) into number NUM <dest> <src>
int command_num(command c, program *p);
// Allocate variable (char array) - fill with zeros
// ALLOC varname size
int command_alloc(command c, program *p);
// APPEND varname numberic_byte_value [0-255]
int command_append(command c, program *p);
int command_location(command c, program *p);
int command_pull(command c, program *p);
int command_push(command c, program *p);
#endif
