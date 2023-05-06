#include "exlang/memory.hpp"
#include "exlang/constants.hpp"
#include "exlang/helpers.hpp"
#include "exlang/macros.hpp"
#ifdef MICRO_DEVICE
#include <Arduino.h>
#else
#include <stdlib.h>
#include <string.h>
#endif

char _memory_area[MAX_MEM] = {0};
_protected _protected_memory[64] = {0, 0, 0};
char _last_err[MAX_LINE_LENGTH] = {0};
extern constant _constants[];

int store_data(const char *cmd) {
	char temp_buffer[MAX_LINE_LENGTH] = {0};
	char location[16] = {0};

	extract(cmd, ' ', 1, location);
	extract(cmd, ' ', 2, temp_buffer);

	unsigned int t = arg_type(temp_buffer);
	long l = atol(location);
	switch (t) {
	case TYPE_STR: {
		char tmp[MAX_LINE_LENGTH] = {0};
		memcpy(tmp, temp_buffer + 1, strlen(temp_buffer) - 2);
		write_area((unsigned int)(l), tmp);
		break;
	}
	case TYPE_DBL: {
		double x = atof(temp_buffer);
		write_area((unsigned int)(l), x);
		break;
	}
	case TYPE_LNG: {
		long x = atol(temp_buffer);
		write_area((unsigned int)(l), x);
		break;
	}
	case TYPE_BYTE: {
		char val = (char)strtol(temp_buffer, 0, 16);
		write_area((unsigned int)(l), val);
		break;
	}
	default: {
		for (unsigned int j = 0; j < CONST_COUNT; j++) {
			if (strcmp(temp_buffer, _constants[j].keyword) == 0) {
				if (_constants[j].is_double) {
					write_area((unsigned int)(l), _constants[j].val);
				} else {
					write_area((unsigned int)(l), _constants[j].vall);
				}

				break;
			}
		}
		break;
	}
	}
	return 0;
}

int init_mem() {
	for (unsigned short i = 0; i < 64; i++) {
		_protected_memory[i].pid = -1;
	}
	return 0;
}

void free_program(char pid) {
	if (pid == 0) {
		// 0 is reserved pid;
		return;
	}
	for (unsigned short i = 0; i < 64; i++) {
		if (_protected_memory[i].pid == pid) {
			_protected_memory[i].pid = -1;
		}
	}
}

void free_area(unsigned int index, unsigned int size) { memset(_memory_area + index, 0, size); }

int write_area(unsigned int index, char *data) {
	if (index == 0) {
		return -1;
	}
	memcpy(_memory_area + index, data, strlen(data));
	return 0;
}

int write_area(unsigned int index, double data) {
	if (index == 0) {
		return -1;
	}
	memcpy(_memory_area + index, &data, sizeof(double));
	return 0;
}

int write_area(unsigned int index, long data) {
	if (index == 0) {
		return -1;
	}
	memcpy(_memory_area + index, &data, sizeof(long));
	return 0;
}

int write_area(unsigned int index, char data) {
	if (index == 0) {
		return -1;
	}
	_memory_area[index] = data;
	return 0;
}

int write_area(unsigned int index, char *data, unsigned int size) {
	if (index == 0) {
		return -1;
	}
	memcpy(_memory_area + index, data, size);
	return 0;
}

int append_area(unsigned int index, char data) {
	for (unsigned int i = index; i < MAX_MEM; i++) {
		if (_memory_area[i] == 0) {
			_memory_area[i] = data;
			return 0;
		}
	}
	return -1;
}

int read_area_str(unsigned int index, unsigned int size, char *back) {
	if (size == 0) {
		unsigned int tmp = 0;
		for (unsigned int i = index; i < MAX_MEM; i++) {
			if (_memory_area[i] != 0) {
				tmp++;
			} else {
				break;
			}
		}
		size = tmp;
	}
	if (size == 0) {
		return 0;
	}
	memcpy(back, _memory_area + index, size);
	return size;
}

double read_area_double(unsigned int index) {
	double *result = (double *)(_memory_area + index);
	return *result;
}

long read_area_long(unsigned int index) {
	long *result = (long *)(_memory_area + index);
	return *result;
}

char read_area_char(unsigned int index) { return _memory_area[index]; }

void error_msg(const char *msg, char pid) {
	UNUSED(pid);
	memset(_last_err, 0, MAX_LINE_LENGTH);
	strcat(_last_err, msg);
}
