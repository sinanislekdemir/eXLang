#include "cmd_variable.hpp"
#include "helpers.hpp"
#include "macros.hpp"

#ifdef MICRO_DEVICE
#include <Arduino.h>
#else
#include <stdio.h>
#include <string.h>
#endif

extern char _memory_area[MAX_MEM];

int command_location(command c, program *p) {
	long pos = get_long(c, 0);
	p->location = (unsigned int)(pos);
	return 0;
}

int command_pull(command c, program *p) {
	int target = c.variable_index[0];
	int size = get_long(c, 1);
	memcpy(_memory_area + target, _memory_area + p->location, size);
	return 0;
}

int command_push(command c, program *p) {
	int source = c.variable_index[0];
	int size = get_long(c, 1);
	memcpy(_memory_area + p->location, _memory_area + source, size);
	return 0;
}

int command_set(command c, program *p) {
	UNUSED(p);
#ifndef DISABLE_EXCEPTIONS
	if (!is_address_type(c.variable_type[0])) {
		error_msg(ERR_STR_INVALID_TYPE, c.pid);
		return -1;
	}
#endif

	if (is_address_type(c.variable_type[1])) {
		char buffer[MAX_LINE_LENGTH] = {0};
		char type = area_type(c.variable_index[1]);
		if (type == TYPE_DBL) {
			double val = read_area_double(c.variable_index[1]);
			write_area(c.variable_index[0], val);
			return 0;
		}
		if (type == TYPE_LNG) {
			long val = read_area_long(c.variable_index[1]);
			write_area(c.variable_index[0], val);
			return 0;
		}
		if (type == TYPE_BYTE) {
			char ch = read_area_char(c.variable_index[1]);
			write_area(c.variable_index[0], ch);
			return 0;
		}
		read_area_str(c.variable_index[1], 0, buffer);
		write_area(c.variable_index[0], buffer);
		return 0;
	} else {
		error_msg(ERR_STR_INVALID_TYPE, c.pid);
		return -1;
	}
	return 0;
}

int command_cpy(command c, program *p) {
	UNUSED(p);
#ifndef DISABLE_EXCEPTIONS
	if (c.arg_count != 3) {
		error_msg(ERR_STR_NOT_ENOUGH_PARAMS, c.pid);
		return -1;
	}
	if (!is_address_type(c.variable_type[0]) || !is_address_type(c.variable_type[1])) {
		error_msg(ERR_STR_INVALID_TYPE, c.pid);
		return -1;
	}
#endif
	char buffer[MAX_LINE_LENGTH] = {0};
	long size = get_long(c, 2);
	read_area_str(c.variable_index[1], (unsigned int)(size), buffer);
	write_area(c.variable_index[0], buffer, size);
	return 0;
}

int command_del(command c, program *p) {
	UNUSED(p);
	if (c.arg_count != 2) {
		error_msg(ERR_STR_NOT_ENOUGH_PARAMS, c.pid);
		return -1;
	}
	if (!is_address_type(c.variable_type[0])) {
		error_msg(ERR_STR_ADDRESS_NOT_FOUND, c.pid);
		return -1;
	}
	long size = get_long(c, 1);
	free_area(c.variable_index[0], (unsigned int)size);
	return 0;
}

int command_alloc(command c, program *p) {
	UNUSED(p);
	if (c.arg_count != 2) {
		error_msg(ERR_STR_NOT_ENOUGH_PARAMS, c.pid);
		return -1;
	}
	if (!is_address_type(c.variable_type[0])) {
		error_msg(ERR_STR_ADDRESS_NOT_FOUND, c.pid);
		return -1;
	}
	double size = get_double(c, 1);
	free_area(c.variable_index[0], (unsigned int)size);
	return 0;
}

int command_append(command c, program *p) {
	UNUSED(p);
#ifndef DISABLE_EXCEPTIONS
	if (!is_address_type(c.variable_type[0])) {
		error_msg(ERR_STR_INVALID_TYPE, c.pid);
		return -1;
	}
#endif
	char byte = get_byte(c, 1);
	return append_area(c.variable_index[0], byte);
}
