#include "cmd_logic.hpp"
#include "helpers.hpp"
#include "macros.hpp"
#ifdef MICRO_DEVICE
#include <Arduino.h>
#else
#include <stdio.h>
#include <string.h>
#endif

extern char _memory_area[MAX_MEM];
extern sub _subs[MAX_SUBS];

int _quick_jump(command c, program *p) {
	if (c.variable_type[0] == TYPE_NUM) {
		int sub = c.variable_index[0];
		int cmd = 0;
		if (c.arg_count == 2) {
			cmd = c.variable_index[1];
		}
		p->append_to_history(p->cursor, _subs[p->subs[p->cursor]].cursor);
		p->cursor = sub;
		_subs[p->subs[p->cursor]].cursor = cmd;
		return 1;
	}
	if (c.variable_type[0] == TYPE_ADDRESS) {
		int sub = get_int(c, 0);
		int cmd = 0;
		if (c.arg_count == 2) {
			cmd = get_int(c, 1);
		}
		p->append_to_history(p->cursor, _subs[p->subs[p->cursor]].cursor);
		p->cursor = sub;
		_subs[p->subs[p->cursor]].cursor = cmd;
		return 1;
	}
	if (c.variable_type[0] != TYPE_LABEL) {
		error_msg(ERR_STR_ADDRESS_NOT_FOUND, p->pid);
		return -1;
	}
	printf("JUMP: %d \n", p->cursor);
	p->append_to_history(p->cursor, _subs[p->subs[p->cursor]].cursor);
	p->cursor = c.variable_index[0];
	_subs[p->subs[p->cursor]].cursor = 0;
	return 1;
}

/**
 * @brief Compare two variables, save result into cmp flag
 *
 * @param c
 * @param p
 * @return int
 */
int command_cmp(command c, program *p) {
#ifndef DISABLE_EXCEPTIONS
	if (c.arg_count != 2) {
		error_msg(ERR_STR_NOT_ENOUGH_PARAMS, c.pid);
		return -1;
	}
#endif
	int n = memcmp(_memory_area + c.variable_index[0], _memory_area + c.variable_index[1], sizeof(double));
	p->set_cmp_flag(n);
	return 0;
}

int command_je(command c, program *p) {
	if (p->_cmp_flag != 0) {
		return 0;
	}
	return _quick_jump(c, p);
}

int command_jne(command c, program *p) {
	if (p->_cmp_flag != 0) {
		return 0;
	}
	return _quick_jump(c, p);
}

int command_jg(command c, program *p) {
	if (p->_cmp_flag <= 0) {
		return 0;
	}
	return _quick_jump(c, p);
}

int command_jge(command c, program *p) {
	if (p->_cmp_flag < 0) {
		return _quick_jump(c, p);
	}
	return 0;
}

int command_jl(command c, program *p) {
	if (p->_cmp_flag >= 0) {
		return 0;
	}
	return _quick_jump(c, p);
}

int command_jle(command c, program *p) {
	if (p->_cmp_flag > 0) {
		return _quick_jump(c, p);
	}
	return 0;
}

int _command_validations(command c) {
#ifndef DISABLE_EXCEPTIONS
	if (c.arg_count != 2) {
		return -1;
	}
	if (c.variable_type[0] != TYPE_ADDRESS) {
		error_msg(ERR_STR_VAR_NOT_FOUND, c.pid);
		return -1;
	}

	if (c.variable_type[1] == TYPE_STR || c.variable_type[1] == TYPE_LABEL) {
		error_msg(ERR_STR_INVALID_TYPE, c.pid);
		return -1;
	}
#endif
	return 0;
}

int command_lrotate(command c, program *p) {
	UNUSED(p);
	int check = _command_validations(c);
	if (check == -1) {
		return check;
	}

	char byte = get_byte(c, 0);
	int bits = get_int(c, 1);
	byte = (byte << bits) | (byte >> (BITS - bits));
	return write_area(c.variable_index[0], byte);
}

int command_rrotate(command c, program *p) {
	UNUSED(p);
	int check = _command_validations(c);
	if (check == -1) {
		return check;
	}
	char byte = get_byte(c, 0);
	int bits = get_int(c, 1);
	byte = (byte >> bits) | (byte << (BITS - bits));
	return write_area(c.variable_index[0], byte);
}

int command_lshift(command c, program *p) {
	UNUSED(p);
	int check = _command_validations(c);
	if (check == -1) {
		return check;
	}
	char byte = get_byte(c, 0);
	int bits = get_int(c, 1);
	byte = byte << bits;
	return write_area(c.variable_index[0], byte);
}

int command_rshift(command c, program *p) {
	UNUSED(p);
	int check = _command_validations(c);
	if (check == -1) {
		return check;
	}
	char byte = get_byte(c, 0);
	int bits = get_int(c, 1);
	byte = byte >> bits;
	return write_area(c.variable_index[0], byte);
}
