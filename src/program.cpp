#include "exlang/program.hpp"
#include "exlang/helpers.hpp"
#include "exlang/interpreter.hpp"
#include "exlang/memory.hpp"
#include "exlang/statement.hpp"

#ifdef MICRO_DEVICE
#include <Arduino.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

sub _subs[MAX_SUBS] = {};
extern char _last_err[MAX_LINE_LENGTH];
extern command commands[MAX_CMDS];

int next_sub() {
	for (unsigned short i = 0; i < MAX_SUBS; i++) {
		if (_subs[i].pid == 0) {
			return i;
		}
	}
	return -1;
}

int sub_command_count(short sub) {
	for (unsigned int i = 0; i < MAX_SUB_COMMANDS; i++) {
		if (_subs[sub].commands[i] == -1) {
			return i;
		}
	}
	return -1;
}

int next_command_index() {
	for (unsigned int i = 0; i < MAX_CMDS; i++) {
		if (commands[i].pid == 0) {
			return i;
		}
	}
	return -1;
}

int sub_next_command(short sub) {
	for (unsigned int i = 0; i < MAX_SUB_COMMANDS; i++) {
		if (_subs[sub].commands[i] == -1) {
			return i;
		}
	}
	return -1;
}

program::program() {

	for (unsigned int i = 0; i < PROG_SUBS; i++) {
		this->back_sub_history[i] = -1;
		this->subs[i] = -1;
	}

#ifdef MICRO_DEVICE
	for (unsigned int i = 0; i < 8; i++) {
		this->interrupts[i].pin = 0;
		this->interrupts[i].state = HIGH;
		this->interrupts[i].triggered = false;
		this->interrupts[i].routine = -1;
	}
#endif
	this->status_code = PROGRAM_FREE;
	this->valid_sub_count = 0;

	this->_sleep = false;
	this->_sleep_start = 0;
	this->_sleep_duration = 0;
	this->location = 0;
	this->pid = 0;
	this->_has_interrupts = false;
}

void program::set_pid(long pid) { this->pid = pid; }

program::~program() { this->destroy(); }

void program::register_interrupt(int pin, int state, char routine) {
	for (unsigned int i = 0; i < 8; i++) {
		if (this->interrupts[i].routine == -1) {
			this->interrupts[i].pin = pin;
			this->interrupts[i].state = state;
			this->interrupts[i].routine = routine;
			this->interrupts[i].triggered = false;
			this->_has_interrupts = true;
			break;
		}
	}
}

void program::destroy() {
	for (unsigned int i = 0; i < PROG_SUBS; i++) {
		if (this->subs[i] == -1)
			continue;
		_subs[this->subs[i]].pid = 0;
		_subs[this->subs[i]].cursor = 0;
		for (unsigned int j = 0; j < 16; j++) {
			_subs[this->subs[i]].back_history[j] = -1;
		}
		for (unsigned int j = 0; j < MAX_SUB_COMMANDS; j++) {
			_subs[this->subs[i]].commands[j] = -1;
		}
		memset(_subs[this->subs[i]].name, 0, 24);
		_subs[this->subs[i]].cursor = 0;
	}
	for (unsigned int i = 0; i < MAX_CMDS; i++) {
		if (commands[i].pid == this->pid) {
			commands[i].arg_count = 0;
			for (unsigned int j = 0; j < 3; j++) {
				commands[i].variable_index[j] = 0;
				commands[i].variable_type[j] = 0;
			}
			commands[i].pid = 0;
			commands[i].statement = 0;
		}
	}
	this->cursor = 0;
	for (unsigned int i = 0; i < PROG_SUBS; i++) {
		this->back_sub_history[i] = -1;
		this->subs[i] = -1;
	}

	for (unsigned int i = 0; i < 8; i++) {
		this->interrupts[i].pin = 0;
		this->interrupts[i].state = 0;
		this->interrupts[i].triggered = false;
		this->interrupts[i].routine = -1;
	}
	this->valid_sub_count = 0;
	this->_sleep = false;
	this->pid = 0;
	free_program(this->pid);
}

void program::set_cmp_flag(int flag) { this->_cmp_flag = flag; }

short program::find_sub(char *name) {
	for (unsigned int i = 0; i < PROG_SUBS; i++) {
		if (strcmp(_subs[this->subs[i]].name, name) == 0 && _subs[this->subs[i]].pid == this->pid) {
			return i;
		}
	}
	return -1;
}

void prepare_all() {
	for (unsigned int i = 0; i < MAX_SUBS; i++) {
		_subs[i].command_count = -1;
		_subs[i].cursor = 0;
		_subs[i].pid = 0;
		for (unsigned k = 0; k < 16; k++) {
			_subs[i].back_history[k] = -1;
		}
		for (unsigned k = 0; k < MAX_SUB_COMMANDS; k++) {
			_subs[i].commands[k] = -1;
		}
		memset(_subs[i].name, 0, 24);
	}
}

short program::pop_sub() {
	short result;
	for (unsigned int i = PROG_SUBS - 1;; i--) {
		if (this->back_sub_history[i] != -1) {
			result = this->back_sub_history[i];
			this->back_sub_history[i] = -1;
			this->cursor = result;
			for (unsigned int j = 15;; j--) {
				if (_subs[this->subs[this->cursor]].back_history[j] != -1) {
					int k = _subs[this->subs[this->cursor]].back_history[j];
					_subs[this->subs[this->cursor]].cursor = k + 1;
					break;
				}
			}
			return result;
		}
	}
	return -1;
}

void program::append_to_history(unsigned short cursor, unsigned short instruction) {
	for (unsigned int i = 0; i < PROG_SUBS; i++) {
		if (this->back_sub_history[i] == -1) {
			this->back_sub_history[i] = cursor;
			break;
		}
	}
	for (unsigned int i = 0; i < 16; i++) {
		if (_subs[this->subs[cursor]].back_history[i] == -1) {
			_subs[this->subs[cursor]].back_history[i] = instruction;
			break;
		}
	}
}

int program::step() {
	int int_status = this->check_interrupts();
	int sub_index = this->subs[this->cursor];
	int cmd_index = _subs[sub_index].commands[_subs[sub_index].cursor];

	if (int_status == 1) {
		this->_sleep = false;
		this->_sleep_start = 0;
		this->_sleep_duration = 0;
		return PROGRAM_RUNNING;
	}

	if (this->_sleep) {
		unsigned long now = _millis();
		if (now - this->_sleep_start >= this->_sleep_duration) {
			this->_sleep = false;
			this->_sleep_start = 0;
			this->_sleep_duration = 0;
		} else {
			return PROGRAM_RUNNING;
		}
	}

	if (this->valid_sub_count == 0) {
		for (unsigned int i = 0; i < PROG_SUBS; i++) {
			if (this->subs[i] == -1) {
				this->valid_sub_count = i + 1;
				break;
			}
		}
	}

	if (this->cursor >= this->valid_sub_count) {
		return PROGRAM_HALT;
	}

	int result = 0;
	if (cmd_index > -1)
		result = run(cmd_index, this);

	if (result == -1) {
		// TODO: raise
		// Check if program has an exception handler
		short handle_sub = this->find_sub((char *)"on_exception");
		if (handle_sub > -1) {
			this->cursor = handle_sub;
			sub_index = this->subs[this->cursor];
			_subs[sub_index].cursor = 0;
			free_area(1, MAX_LINE_LENGTH);
			write_area(1, _last_err);
			return PROGRAM_RUNNING;
		}
		return PROGRAM_HALT;
	}
	if (result == -2) {
		return PROGRAM_HALT;
	}

	if (result == 1) {
		// Cursor is explicitly set by the instruction
		return PROGRAM_RUNNING;
	}
	_subs[sub_index].cursor++;

	if (_subs[sub_index].command_count == -1) {
		_subs[sub_index].command_count = sub_command_count(sub_index);
	}
	if (_subs[sub_index].cursor >= (unsigned short)_subs[sub_index].command_count) {
		this->pop_sub();
		if (this->cursor == -1) {
			return PROGRAM_HALT;
		}
	}

	return PROGRAM_RUNNING;
}

int program::compile(char *line) {
	ltrim(line);
	size_t l = strlen(line);
	if (l < 3 || line[0] == '#') {
		return 0;
	}

	// Create a new sub
	if (line[l - 1] == ':') {
		int n = next_sub();
		if (n == -1) {
			return -1;
		}

		if (n > 0) {
			int sub_index = this->subs[this->_compile_cursor];
			_subs[sub_index].command_count = sub_command_count(sub_index);
		}

		short free_sub = -1;
		for (unsigned short i = 0; i < PROG_SUBS; i++) {
			if (this->subs[i] == -1) {
				free_sub = i;
				break;
			}
		}
		this->_compile_cursor = free_sub;
		this->subs[free_sub] = n;

		memset(_subs[n].name, 0, 24);
		strncpy(_subs[n].name, line, l - 1);
		if (strcmp(line, "main:") == 0) {
			this->cursor = this->_compile_cursor;
		}
		for (unsigned short i = 0; i < MAX_SUB_COMMANDS; i++) {
			_subs[n].commands[i] = -1;
		}
		for (unsigned short i = 0; i < 16; i++) {
			_subs[n].back_history[i] = -1;
		}
		_subs[n].pid = this->pid;
		_subs[n].cursor = 0;
		_subs[n].command_count = -1;
		return 0;
	}
	// End the existing sub
	int sub_index = this->subs[this->_compile_cursor];
	int nci = next_command_index();
	int snc = sub_next_command(sub_index);
	int check = parse(line, this->pid, nci);
	if (check < 0) {
		return -1;
	}

	_subs[sub_index].commands[snc] = nci;
	return 0;
}

int program::check_interrupts() {
	// Return 1 if it jumps
	if (!this->_has_interrupts)
		return 0;
#ifdef MICRO_DEVICE
	for (unsigned int i = 0; i < 8; i++) {
		if (this->interrupts[i].pin == 0) {
			continue;
		}

		int val = digitalRead(this->interrupts[i].pin);

		if (val == this->interrupts[i].state && this->interrupts[i].triggered == false) {
			this->interrupts[i].triggered = true;
			this->append_to_history(this->cursor, _subs[this->subs[this->cursor]].cursor);
			this->cursor = this->interrupts[i].routine;
			_subs[this->subs[this->cursor]].cursor = 0;
			return 1;
		}
	}

	return 0;
#else
	return 0;
#endif
}

int program::parse(const char *cmd, unsigned int pid, int index) {
	unsigned int argument_count = argc(cmd, ' ') - 1;
	char temp_buffer[MAX_LINE_LENGTH] = {0};
	extract(cmd, ' ', 0, temp_buffer);

	if (argument_count > 3) {
		argument_count = 3;
	}

	char st = find_statement((const char *)temp_buffer);
	if (st == 0) {
		error_msg("Unknown command", pid);
		return -1;
	}

	if (st == STATEMENT_DATA) {
		if (argument_count != 2) {
			return -1;
		}
		store_data(cmd);
		st = STATEMENT_NOOP;
		argument_count = 0;
	}

	commands[index].statement = st;
	commands[index].pid = pid;
	commands[index].exception = false;
	commands[index].arg_count = argument_count;

	for (unsigned int i = 0; i < argument_count; i++) {
		memset(temp_buffer, 0, MAX_LINE_LENGTH);
		int check = extract(cmd, ' ', i + 1, temp_buffer);
		if (check == 0) {
			break;
		}

		short t = arg_type(temp_buffer);
		commands[index].variable_type[i] = t;

		if (t == TYPE_LNG &&
		    (st == STATEMENT_JE || st == STATEMENT_JG || st == STATEMENT_JL || st == STATEMENT_JGE || st == STATEMENT_JLE ||
		     st == STATEMENT_JNE || st == STATEMENT_GOTO || st == STATEMENT_CALL || st == STATEMENT_SYS)) {
			commands[index].variable_index[i] = atoi(temp_buffer);
			continue;
		}

		if (t == TYPE_LNG || t == TYPE_DBL || t == TYPE_STR || t == TYPE_BYTE) {
			error_msg("DATA is not initialized", 0);
			return -1;
		}

		if (is_address_type(t)) {
			temp_buffer[strlen(temp_buffer) - 1] = '\0';
			unsigned short loc = atoi(temp_buffer);
			commands[index].variable_index[i] = loc; // unknown yet
		}

		if (t == TYPE_LABEL) {
			short sub = this->find_sub(temp_buffer);
			if (sub == -1) {
				error_msg("Sub not defined (yet)", 0);
				return -1;
			}
			commands[index].variable_index[i] = sub;
		}
	}
	return 0;
}
