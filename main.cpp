#include "program.hpp"
#include "statement.hpp"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>

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
	char buffer[1024];
	while (getline(file, tp)) {
		memset(buffer, 0, 1024);
		strcat(buffer, tp.c_str());
		prog.compile(buffer);
	}
	prog.status_code = PROGRAM_RUNNING;
	int stat = PROGRAM_RUNNING;
	while (stat == PROGRAM_RUNNING) {
		stat = prog.step();
	}
	return 0;
}
