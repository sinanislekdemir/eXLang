#ifndef EXLANG_PROGRAM_HPP
#define EXLANG_PROGRAM_HPP

#ifndef MAX_SUB_COMMANDS
#define MAX_SUB_COMMANDS 36 // 128 in ESP32
#endif
#ifndef MAX_SUBS
#define MAX_SUBS 8 // 32 in ESP32
#endif
#ifndef PROG_SUBS
#define PROG_SUBS 8 // 32 in ESP32
#endif
#ifndef MAX_PROGS
#define MAX_PROGS 4 // 16 in ESP32
#endif
#ifndef MAX_ALIAS
#define MAX_ALIAS 32
#endif

#define XSTR(x) STR(x)
#define STR(x) #x

#include "exlang/command.hpp"
#include "exlang/constants.hpp"

void prepare_all();

struct sub {
	short back_history[16];
	short commands[MAX_SUB_COMMANDS];
	char name[24];
	unsigned short cursor;
	short command_count;
	long pid;
};

struct interrupt {
	int pin;
	int state;
	bool triggered;
	short routine;
};

class program {
      private:
	bool _serial_input;
	char valid_sub_count;

	short _compile_cursor;
	int _alias_cursor;
	bool _has_interrupts;

	int parse(const char *cmd, unsigned int pid, int index);

      public:
	int _cmp_flag;
	short back_sub_history[PROG_SUBS];
	short cursor;
	short subs[PROG_SUBS];

	interrupt interrupts[8];

	bool _sleep;
	unsigned long _sleep_start;
	unsigned long _sleep_duration;
	unsigned int core;
	unsigned int location;

	short find_sub(char *name);
	void append_to_history(unsigned short cursor, unsigned short instruction);
	void register_interrupt(int pin, int state, char routine);
	int check_interrupts();
	short pop_sub();

	long pid;
	char status_code;
	unsigned int priority;
	unsigned long start_time;
	unsigned long end_time;

	int compile(char *line);
	void set_pid(long pid);
	program();
	~program();
	int step();
	void destroy();
	void set_cmp_flag(int flag);
};

#endif
