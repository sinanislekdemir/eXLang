#include "program.hpp"
#include "statement.hpp"
#ifdef MICRO_DEVICE
#include <Arduino.h>
#else
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#endif

extern char _last_err[MAX_LINE_LENGTH];
#ifndef MICRO_DEVICE
using namespace std;
#endif

#ifdef MICRO_DEVICE
int serial_lock = 0;
program PROGS[MAX_PROGS];
bool step_tasks(int core);

int _core(int index) {
#ifdef BOARD_ESP32
	return index % 2;
#else
	return 0;
#endif
}

#ifdef BOARD_ESP32
TaskHandle_t task_handles[2];
void core_run(void *_core) {
	unsigned int core = (unsigned int)(ulong)(_core);
	while (step_tasks(core)) {
		yield();
	}
}
#endif

void setup() {
	char buffer[128];
	Serial.begin(9600);
	register_statements();
	prepare_all();
	int c = 0;
	for (unsigned int index = 0; index < MAX_PROGS; index++) {
		PROGS[index] = program();
		PROGS[index].pid = index + 1;
		PROGS[index].core = _core(index);
		PROGS[index].cursor = 0;
		PROGS[index].status_code = PROGRAM_FREE;
		PROGS[index].start_time = 0;
		PROGS[index].end_time = 0;
		Serial.print("Receiving source code for ");
		Serial.println(index);
		Serial.println("End program with a single dot");
		while (strcmp(buffer, ".") != 0) {
			memset(buffer, 0, 128);
			Serial.readBytesUntil('\n', buffer, 128);
			if (buffer[0] != '.' && PROGS[index].status_code != PROGRAM_COMPILING) {
				PROGS[index].status_code = PROGRAM_COMPILING;
			}
			c = PROGS[index].compile(buffer);
			if (c < 0) {
				Serial.print("ERR: ");
				Serial.println(_last_err);
				PROGS[index].status_code = PROGRAM_HALT;
				PROGS[index].destroy();
				break;
			}
		}
	}
	for (unsigned int i = 0; i < MAX_PROGS; i++) {
		if (PROGS[i].status_code == PROGRAM_COMPILING) {
			PROGS[i].status_code = PROGRAM_RUNNING;
		}
	}
#ifdef BOARD_ESP32
	char core_id[2] = {0};
	for (int i = 0; i < 2; i++) {
		char name[8] = {0};
		itoa(i, core_id, 10);
		strcat(name, "core_");
		strcat(name, core_id);
		xTaskCreatePinnedToCore(core_run, name, 8196, (void *)i, 1, &task_handles[i], i);
	}
#else
	while (step_tasks(0)) {
	}
#endif
}

bool step_tasks(int core) {
#ifdef BOARD_ESP32
	vTaskDelay(1);
#endif
	for (unsigned int index = 0; index < MAX_PROGS; index++) {
		if (PROGS[index].status_code != PROGRAM_RUNNING) {
			continue;
		}
		if (PROGS[index].core != core) {
			continue;
		}
		if (PROGS[index].start_time == 0) {
			PROGS[index].start_time = millis();
		}
		int result = PROGS[index].step();
#ifdef BOARD_ESP32
		vTaskDelay(1);
#endif
		if (result != PROGRAM_RUNNING) {
			PROGS[index].status_code = PROGRAM_FREE;
			PROGS[index].end_time = millis();
			PROGS[index].pid = 0;
			PROGS[index].destroy();
		}
	}
	return true;
}

void loop() {}
#else
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
#endif