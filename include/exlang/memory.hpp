#ifndef EXLANG_MEMORY_HPP
#define EXLANG_MEMORY_HPP

#include "exlang/constants.hpp"

#ifndef MAX_MEM
#define MAX_MEM 1024 // 1024 * 8 in ESP32 // NOLINT
#endif

#ifdef BOARD_ATMEGA
#define BITS 8 // NOLINT
#elif BOARD_ESP32
#define BITS 32 // NOLINT
#else
#define BITS 8 // NOLINT
#endif

// jump request
struct jump_request {
	int request;
	char pid;
	short label;
};

struct _protected {
	short pid;
	unsigned int from;
	unsigned int to;
};

int store_data(const char *cmd);
void free_area(unsigned int index, unsigned int size);
void free_program(char pid);

int write_area(unsigned int index, char *data);
int write_area(unsigned int index, char *data, unsigned int size);
int write_area(unsigned int index, double data);
int write_area(unsigned int index, long data);
int write_area(unsigned int index, char data);
int append_area(unsigned int index, char data);
int read_area_str(unsigned int index, unsigned int size, char *back);
double read_area_double(unsigned int index);
long read_area_long(unsigned int index);
char read_area_char(unsigned int index);

void error_msg(const char *msg, char pid);

#endif
