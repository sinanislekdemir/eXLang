#include "cmd_math.hpp"
#include "constants.hpp"
#include "helpers.hpp"
#include "macros.hpp"
#include <math.h>
#ifdef MICRO_DEVICE
#include <Arduino.h>
#else
#include <stdio.h>
#include <stdlib.h>
#endif

int command_inc(command c, program *p) {
	UNUSED(p);
	write_area(c.variable_index[0], read_area_double(c.variable_index[0]) + 1);
        return 0;
}

int command_num(command c, program *p) {
	UNUSED(p);
	char str[MAX_LINE_LENGTH];
	read_area_str(c.variable_index[1], MAX_LINE_LENGTH, str);
	return write_area(c.variable_index[0], atof(str));
}

int command_rand(command c, program *p) {
	UNUSED(p);
	// RAND @target @from @to
	int from = int(read_area_double(c.variable_index[1]));
	int to = int(read_area_double(c.variable_index[2]));
#ifdef MICRO_DEVICE
	double rn = double(random(from, to));
#else
	to += from;
	double rn = double(random() % to);
#endif
	return write_area(c.variable_index[0], rn);
}

int command_add(command c, program *p) {
	UNUSED(p);

	return write_area(c.variable_index[0], read_area_double(c.variable_index[1]) + read_area_double(c.variable_index[2]));
}

int command_sub(command c, program *p) {
	UNUSED(p);

	return write_area(c.variable_index[0], read_area_double(c.variable_index[1]) - read_area_double(c.variable_index[2]));
}

int command_div(command c, program *p) {
	UNUSED(p);

	return write_area(c.variable_index[0], read_area_double(c.variable_index[1]) / read_area_double(c.variable_index[2]));
}

int command_mul(command c, program *p) {
	UNUSED(p);

	return write_area(c.variable_index[0], read_area_double(c.variable_index[1]) * read_area_double(c.variable_index[2]));
}

int command_xor(command c, program *p) {
	UNUSED(p);

	return write_area(c.variable_index[0],
			  double(int(read_area_double(c.variable_index[1])) % int(read_area_double(c.variable_index[2]))));
}

int command_or(command c, program *p) {
	UNUSED(p);

	return write_area(c.variable_index[0],
			  double(int(read_area_double(c.variable_index[1])) | int(read_area_double(c.variable_index[2]))));
}

int command_and(command c, program *p) {
	UNUSED(p);

	return write_area(c.variable_index[0],
			  double(int(read_area_double(c.variable_index[1])) & int(read_area_double(c.variable_index[2]))));
}

int command_pow(command c, program *p) {
	UNUSED(p);

	return write_area(c.variable_index[0], pow(read_area_double(c.variable_index[1]), read_area_double(c.variable_index[2])));
}

int command_trigonometry(command c, program *p) {
	UNUSED(p);
	if (c.variable_type[0] != TYPE_ADDRESS) {
		error_msg(ERR_STR_INVALID_TYPE, c.pid);
		return -1;
	}

	double val = get_double(c, 1);
	double res = 0;

	switch (c.statement) {
	case STATEMENT_SIN:
		res = sin(val);
		break;
	case STATEMENT_COS:
		res = cos(val);
		break;
	case STATEMENT_TAN:
		res = tan(val);
		break;
	case STATEMENT_COT:
		res = 1.0 / tan(val);
		break;
	case STATEMENT_SINH:
		res = sinh(val);
		break;
	case STATEMENT_COSH:
		res = cosh(val);
		break;
	case STATEMENT_TANH:
		res = tanh(val);
		break;
	case STATEMENT_COTH:
		res = 1.0 / tanh(val);
		break;
	case STATEMENT_ASIN:
		res = asin(val);
		break;
	case STATEMENT_ACOS:
		res = acos(val);
		break;
	case STATEMENT_ATAN:
		res = atan(val);
		break;
	case STATEMENT_ACOT:
		res = 1.0 / atan(val);
		break;
	default:
		res = 0;
		break;
	}

	return write_area(c.variable_index[0], res);
}
