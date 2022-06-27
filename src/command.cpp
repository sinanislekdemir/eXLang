#include "command.hpp"
#include "constants.hpp"

command commands[MAX_CMDS] = {};

double get_double(command c, int index) {
	if (c.variable_type[index] == TYPE_ADDRESS_DBL)
		return read_area_double(c.variable_index[index]);
	return double(read_area_long(c.variable_index[index]));
}

long get_long(command c, int index) {
	if (c.variable_type[index] == TYPE_ADDRESS_LNG)
		return read_area_long(c.variable_index[index]);
	return long(read_area_double(c.variable_index[index]));
}

void get_string(command c, int index, char *back, unsigned int size) { read_area_str(c.variable_index[index], size, back); }

char get_byte(command c, int index) { return read_area_char(c.variable_index[index]); }
