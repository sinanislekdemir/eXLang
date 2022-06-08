#include "program.hpp"
#include "statement.hpp"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>

extern char _last_err[MAX_LINE_LENGTH];
using namespace std;

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("./exlang <script-filename>\n");
		return 0;
	}
	fstream file;
	file.open(argv[1], ios::in);
	if (!file.is_open()) {
		printf("Failed to open file %s\n", argv[1]);
		return 1;
	}
	printf("Register statements: ");
	register_statements();
	printf(" done\nPrepare environment: ");
	prepare_all();
	printf(" done\n");

	program prog = program();
	prog.pid = 1;
	prog.core = 0;
	prog.cursor = 0;

	prog.status_code = PROGRAM_COMPILING;
	prog.start_time = 0;
	prog.end_time = 0;
	string tp;
	int c = 0;
	char buffer[1024];
	while (getline(file, tp)) {
		memset(buffer, 0, 1024);
		strcat(buffer, tp.c_str());
		c = prog.compile(buffer);
		if (c < 0) {
			printf("ERR: [%s]\n", _last_err);
			return c;
		}
	}
	prog.status_code = PROGRAM_RUNNING;
	int stat = PROGRAM_RUNNING;
	while (stat == PROGRAM_RUNNING) {
		stat = prog.step();
	}
	if (stat == -1) {
		printf("ERR: [%s]\n", _last_err);
	}
	return 0;
}
