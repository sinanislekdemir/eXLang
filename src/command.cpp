#include "command.hpp"
#include "constants.hpp"

command commands[MAX_CMDS] = {};

double get_double(command c, int index) { return read_area_double(c.variable_index[index]); }

int get_int(command c, int index) { return read_area_int(c.variable_index[index]); }

long get_long(command c, int index) { return read_area_long(c.variable_index[index]); }

void get_string(command c, int index, char *back, unsigned int size) { read_area_str(c.variable_index[index], size, back); }

char get_byte(command c, int index) { return read_area_char(c.variable_index[index]); }
