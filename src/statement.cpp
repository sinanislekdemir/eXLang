#include "exlang/statement.hpp"
#include "exlang/cmd_analog.hpp"
#include "exlang/cmd_digital.hpp"
#include "exlang/cmd_interrupts.hpp"
#include "exlang/cmd_log.hpp"
#include "exlang/cmd_logic.hpp"
#include "exlang/cmd_math.hpp"
#include "exlang/cmd_navigate.hpp"
#include "exlang/cmd_serial.hpp"
#include "exlang/cmd_system.hpp"
#include "exlang/cmd_time.hpp"
#include "exlang/cmd_variable.hpp"
#include "exlang/constants.hpp"
#include "exlang/helpers.hpp"
#ifdef MICRO_DEVICE
#include <Arduino.h>
#else
#include <string.h>
#endif

statement statements[NUM_STATEMENTS] = {0, 0};

void add_statement(char cmd, int (*f)(command c, program *p), unsigned int index) {
	statements[index].command = cmd;
	statements[index].f = f;
}

/**
   This looks like a stupid function in the beginning
   because it returns the same string given BUT
   in-fact it's returning a memory location from
   stack, not heap. So, it's memory efficient when
   commands are repeating.
 */
char find_statement(const char *cmd) {
	if (strcmp(cmd, "SPRINT") == 0)
		return STATEMENT_SPRINT;
	if (strcmp(cmd, "SPRINTLN") == 0)
		return STATEMENT_SPRINTLN;
	if (strcmp(cmd, "SREADLN") == 0)
		return STATEMENT_SREADLN;
	if (strcmp(cmd, "GOTO") == 0)
		return STATEMENT_GOTO;
	if (strcmp(cmd, "CALL") == 0)
		return STATEMENT_CALL;
	if (strcmp(cmd, "JE") == 0)
		return STATEMENT_JE;
	if (strcmp(cmd, "JNE") == 0)
		return STATEMENT_JNE;
	if (strcmp(cmd, "JG") == 0)
		return STATEMENT_JG;
	if (strcmp(cmd, "JGE") == 0)
		return STATEMENT_JGE;
	if (strcmp(cmd, "JL") == 0)
		return STATEMENT_JL;
	if (strcmp(cmd, "JLE") == 0)
		return STATEMENT_JLE;
	if (strcmp(cmd, "CMP") == 0)
		return STATEMENT_CMP;
	if (strcmp(cmd, "LOG") == 0)
		return STATEMENT_LOG;
	if (strcmp(cmd, "ADD") == 0)
		return STATEMENT_ADD;
	if (strcmp(cmd, "SUB") == 0)
		return STATEMENT_SUB;
	if (strcmp(cmd, "DIV") == 0)
		return STATEMENT_DIV;
	if (strcmp(cmd, "MUL") == 0)
		return STATEMENT_MUL;
	if (strcmp(cmd, "XOR") == 0)
		return STATEMENT_XOR;
	if (strcmp(cmd, "OR") == 0)
		return STATEMENT_OR;
	if (strcmp(cmd, "AND") == 0)
		return STATEMENT_AND;
	if (strcmp(cmd, "POW") == 0)
		return STATEMENT_POW;
	if (strcmp(cmd, "SIN") == 0)
		return STATEMENT_SIN;
	if (strcmp(cmd, "COS") == 0)
		return STATEMENT_COS;
	if (strcmp(cmd, "TAN") == 0)
		return STATEMENT_TAN;
	if (strcmp(cmd, "COT") == 0)
		return STATEMENT_COT;
	if (strcmp(cmd, "SINH") == 0)
		return STATEMENT_SINH;
	if (strcmp(cmd, "COSH") == 0)
		return STATEMENT_COSH;
	if (strcmp(cmd, "TANH") == 0)
		return STATEMENT_TANH;
	if (strcmp(cmd, "COTH") == 0)
		return STATEMENT_COTH;
	if (strcmp(cmd, "ASIN") == 0)
		return STATEMENT_ASIN;
	if (strcmp(cmd, "ACOS") == 0)
		return STATEMENT_ACOS;
	if (strcmp(cmd, "ATAN") == 0)
		return STATEMENT_ATAN;
	if (strcmp(cmd, "ACOT") == 0)
		return STATEMENT_ACOT;
	if (strcmp(cmd, "HALT") == 0)
		return STATEMENT_HALT;
	if (strcmp(cmd, "ALLOC") == 0)
		return STATEMENT_ALLOC;
	if (strcmp(cmd, "APPEND") == 0)
		return STATEMENT_APPEND;
	if (strcmp(cmd, "SET") == 0)
		return STATEMENT_SET;
	if (strcmp(cmd, "DEL") == 0)
		return STATEMENT_DEL;
	if (strcmp(cmd, "CPY") == 0)
		return STATEMENT_CPY;
	if (strcmp(cmd, "LROTATE") == 0)
		return STATEMENT_LROTATE;
	if (strcmp(cmd, "RROTATE") == 0)
		return STATEMENT_RROTATE;
	if (strcmp(cmd, "LSHIFT") == 0)
		return STATEMENT_LSHIFT;
	if (strcmp(cmd, "RSHIFT") == 0)
		return STATEMENT_RSHIFT;
	if (strcmp(cmd, "SLEEP") == 0)
		return STATEMENT_SLEEP;
	if (strcmp(cmd, "MILLIS") == 0)
		return STATEMENT_MILLIS;
#ifdef MICRO_DEVICE
	if (strcmp(cmd, "INT") == 0)
		return STATEMENT_INT;
#endif
	if (strcmp(cmd, "CORE") == 0)
		return STATEMENT_CORE;
	if (strcmp(cmd, "SYS") == 0)
		return STATEMENT_SYS;
#ifdef MICRO_DEVICE
	if (strcmp(cmd, "AREAD") == 0)
		return STATEMENT_AREAD;
	if (strcmp(cmd, "AWRITE") == 0)
		return STATEMENT_AWRITE;
	if (strcmp(cmd, "PINMODE") == 0)
		return STATEMENT_PINMODE;
	if (strcmp(cmd, "DWRITE") == 0)
		return STATEMENT_DWRITE;
	if (strcmp(cmd, "DREAD") == 0)
		return STATEMENT_DREAD;
#endif
	if (strcmp(cmd, "NOOP") == 0)
		return STATEMENT_NOOP;
	if (strcmp(cmd, "DATA") == 0)
		return STATEMENT_DATA;
	if (strcmp(cmd, "RAND") == 0)
		return STATEMENT_RAND;
	if (strcmp(cmd, "NUM") == 0)
		return STATEMENT_NUM;
	if (strcmp(cmd, "LOCATE") == 0)
		return STATEMENT_LOCATE;
	if (strcmp(cmd, "PULL") == 0)
		return STATEMENT_PULL;
	if (strcmp(cmd, "PUSH") == 0)
		return STATEMENT_PUSH;
	if (strcmp(cmd, "INC") == 0)
		return STATEMENT_INC;
	return 0;
}

void register_statements() {
	/**
	   ATTENTION!
	   Order of lines below would greatly influence execution speed;
	   Keep the most common commands on top
	 */
	add_statement(STATEMENT_SPRINT, command_serial_print, 0);
	add_statement(STATEMENT_SPRINTLN, command_serial_println, 1);
	add_statement(STATEMENT_SREADLN, command_getln, 2);
	add_statement(STATEMENT_GOTO, command_goto, 3);
	add_statement(STATEMENT_CALL, command_call, 4);

	add_statement(STATEMENT_JE, command_je, 5);
	add_statement(STATEMENT_JNE, command_jne, 6);
	add_statement(STATEMENT_JG, command_jg, 7);
	add_statement(STATEMENT_JGE, command_jge, 8);
	add_statement(STATEMENT_JL, command_jl, 9);
	add_statement(STATEMENT_JLE, command_jle, 10);
	add_statement(STATEMENT_CMP, command_cmp, 11);

	add_statement(STATEMENT_LOG, command_log, 12);

	add_statement(STATEMENT_ADD, command_add, 13);
	add_statement(STATEMENT_SUB, command_sub, 14);
	add_statement(STATEMENT_DIV, command_div, 15);
	add_statement(STATEMENT_MUL, command_mul, 16);
	add_statement(STATEMENT_XOR, command_xor, 17);
	add_statement(STATEMENT_OR, command_or, 18);
	add_statement(STATEMENT_AND, command_and, 19);
	add_statement(STATEMENT_POW, command_pow, 20);
	add_statement(STATEMENT_SIN, command_trigonometry, 21);
	add_statement(STATEMENT_COS, command_trigonometry, 22);
	add_statement(STATEMENT_TAN, command_trigonometry, 23);
	add_statement(STATEMENT_COT, command_trigonometry, 24);
	add_statement(STATEMENT_SINH, command_trigonometry, 25);
	add_statement(STATEMENT_COSH, command_trigonometry, 26);
	add_statement(STATEMENT_TANH, command_trigonometry, 27);
	add_statement(STATEMENT_COTH, command_trigonometry, 28);
	add_statement(STATEMENT_ASIN, command_trigonometry, 29);
	add_statement(STATEMENT_ACOS, command_trigonometry, 30);
	add_statement(STATEMENT_ATAN, command_trigonometry, 31);
	add_statement(STATEMENT_ACOT, command_trigonometry, 32);

	add_statement(STATEMENT_HALT, command_halt, 33);
	add_statement(STATEMENT_ALLOC, command_alloc, 34);
	add_statement(STATEMENT_APPEND, command_append, 35);
	add_statement(STATEMENT_SET, command_set, 36);
	add_statement(STATEMENT_DEL, command_del, 37);
	add_statement(STATEMENT_NOOP, command_noop, 38);
	add_statement(STATEMENT_CPY, command_cpy, 39);
	add_statement(STATEMENT_LROTATE, command_lrotate, 40);
	add_statement(STATEMENT_RROTATE, command_rrotate, 41);
	add_statement(STATEMENT_LSHIFT, command_lshift, 42);
	add_statement(STATEMENT_RSHIFT, command_rshift, 43);

	add_statement(STATEMENT_SLEEP, command_sleep, 44);
	add_statement(STATEMENT_MILLIS, command_millis, 45);
	add_statement(STATEMENT_CORE, command_core, 46);
	add_statement(STATEMENT_SYS, command_sys, 47);

#ifdef MICRO_DEVICE
	add_statement(STATEMENT_INT, command_int, 48);
	add_statement(STATEMENT_AREAD, command_analogread, 49);
	add_statement(STATEMENT_AWRITE, command_analogwrite, 50);
	add_statement(STATEMENT_PINMODE, command_pinmode, 51);
	add_statement(STATEMENT_DWRITE, command_digitalwrite, 52);
	add_statement(STATEMENT_DREAD, command_digitalread, 53);
#endif
	add_statement(STATEMENT_DATA, command_noop, 54);
	add_statement(STATEMENT_RAND, command_rand, 55);
	add_statement(STATEMENT_NUM, command_num, 56);
	add_statement(STATEMENT_LOCATE, command_location, 57);
	add_statement(STATEMENT_PULL, command_pull, 58);
	add_statement(STATEMENT_PUSH, command_push, 59);
	add_statement(STATEMENT_INC, command_inc, 60);
}
