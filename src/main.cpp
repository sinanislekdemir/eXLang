#include "exlang/program.hpp"
#include "exlang/statement.hpp"
#ifdef MICRO_DEVICE
#include <Arduino.h>
#else
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#endif

extern sub _subs[MAX_SUBS];

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
	int c = 0;
	while (step_tasks(core)) {
#ifdef BOARD_ESP32
		if (c++ > 1000) {
			vTaskDelay(1 / portTICK_PERIOD_MS);
			c = 0;
		}
#endif
		yield();
	}
}
#endif

void _readline(char *back, size_t size) {
	char b = 0;
	memset(back, 0, size);
	unsigned int i = 0;
	while (b != '\n') {
		if (!Serial.available()) {
			continue;
		}
		b = Serial.read();
		if (b == '\n')
			break;
		if (b > 31 && b < 127) {
			back[i++] = b;
			Serial.print(b);
		}
	}
	Serial.print('\n');
}

void setup() {
	char buffer[128];
	Serial.begin(9600);
	register_statements();
	prepare_all();
#ifdef BOARD_ATMEGA
	Serial.println("ATMEL");
#endif
#ifdef BOARD_ESP32
	while (!Serial.available()) {
		vTaskDelay(1 / portTICK_PERIOD_MS);
	}
	Serial.println("ESP32");
#endif
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
			_readline(buffer, 128);
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
		memset(buffer, 0, 128);
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
		if (result != PROGRAM_RUNNING) {
			PROGS[index].status_code = PROGRAM_FREE;
			PROGS[index].end_time = millis();
			PROGS[index].pid = 0;
			PROGS[index].destroy();
		}
	}

	return true;
}

void loop() {
#ifdef BOARD_ESP32
	vTaskDelete(NULL);
#endif
}
#else
int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("./exlang <script-filename>\n");
		return 0;
	}
	if (strcmp(argv[1], "--version") == 0) {
		printf("eXLang 1.0.0\n");
		return 0;
	}
	fstream file;
	file.open(argv[1], ios::in);
	if (!file.is_open()) {
		printf("Failed to open file %s\n", argv[1]);
		return 1;
	}

	register_statements();
	prepare_all();

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
