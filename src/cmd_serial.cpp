#include "exlang/cmd_serial.hpp"
#include "exlang/constants.hpp"
#include "exlang/helpers.hpp"
#include "exlang/macros.hpp"
#ifdef MICRO_DEVICe
#include <Arduino.h>
#else
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#endif

#ifdef MICRO_DEVICE
extern int serial_lock;
#else
int serial_lock = 0;
#endif

bool _breaker(char c) { return c == '\n'; }
bool _ignore(char c) { return (c < 32); }

int _serial_getline(char *result, unsigned int buffer_length) {
#ifdef MICRO_DEVICE
	char c = 0;
	unsigned int cursor = 0;
	while (strlen(result) < buffer_length) {
		if (!Serial.available()) {
#ifdef BOARD_ESP32
			vTaskDelay(1 / portTICK_PERIOD_MS);
#endif
			continue;
		}
		c = Serial.read();
		if (_breaker(c)) {
			break;
		}
		if (_ignore(c)) {
			continue;
		}
		Serial.print(c);
		result[cursor++] = c;
	}
	return 0;
#else
	size_t bl = size_t(buffer_length);

	getline(&result, &bl, stdin);
	if (result[strlen(result) - 1] == '\n') {
		result[strlen(result) - 1] = '\0';
	}

	return 0;
#endif
}

int command_serial_println(command c, program *p) {
	UNUSED(p);
	char buffer[MAX_LINE_LENGTH] = {0};

	char type = c.variable_type[0];
	if (type == TYPE_ADDRESS_LNG) {
#ifdef MICRO_DEVICE
		Serial.println(get_long(c, 0));
#else
		printf("%ld\n", get_long(c, 0));
#endif
	} else if (type == TYPE_ADDRESS_DBL) {
#ifdef MICRO_DEVICE
		Serial.println(get_double(c, 0));
#else
		printf("%f\n", get_double(c, 0));
#endif
	} else if (type == TYPE_ADDRESS_CHR) {
#ifdef MICRO_DEVICE
		Serial.println(int(get_byte(c, 0)));
#else
		printf("%d\n", int(get_byte(c, 0)));
#endif
	} else {
		get_string(c, 0, buffer, 0);
#ifdef MICRO_DEVICE
		Serial.println(buffer);
#else
		printf("%s\n", buffer);
#endif
	}

	return 0;
}

int command_serial_print(command c, program *p) {
	UNUSED(p);
	char buffer[MAX_LINE_LENGTH] = {0};

	char type = c.variable_type[0];
	if (type == TYPE_ADDRESS_LNG) {
#ifdef MICRO_DEVICE
		Serial.print(get_long(c, 0));
#else
		printf("%ld", get_long(c, 0));
#endif
	} else if (type == TYPE_ADDRESS_DBL) {
#ifdef MICRO_DEVICE
		Serial.print(get_double(c, 0));
#else
		printf("%f", get_double(c, 0));
#endif
	} else if (type == TYPE_ADDRESS_CHR) {
#ifdef MICRO_DEVICE
		Serial.print(int(get_byte(c, 0)));
#else
		printf("%d", int(get_byte(c, 0)));
#endif
	} else {
		get_string(c, 0, buffer, 0);
#ifdef MICRO_DEVICE
		Serial.print(buffer);
#else
		printf("%s", buffer);
#endif
	}

	return 0;
}

int command_getln(command c, program *p) {
#ifndef DISABLE_EXCEPTIONS
	if (c.variable_type[0] != TYPE_ADDRESS_STR) {
		error_msg(ERR_STR_INVALID_TYPE, c.pid);
		return -1;
	}
	if (c.arg_count < 2) {
		error_msg(ERR_STR_NOT_ENOUGH_PARAMS, c.pid);
		return -1;
	}
#endif

	unsigned int buf_size = MAX_LINE_LENGTH;
	if (c.arg_count == 2)
		buf_size = get_long(c, 1);
	if (buf_size > MAX_LINE_LENGTH) {
		buf_size = MAX_LINE_LENGTH;
	}

	char buffer[MAX_LINE_LENGTH] = {0};

	while (serial_lock > 0) {
#ifdef MICRO_DEVICE
		delay(100); // wait for the lock
#else
		usleep(100);
#endif
	}

	serial_lock = p->pid;
	_serial_getline(buffer, buf_size);
	serial_lock = -1;

	return write_area(c.variable_index[0], buffer, buf_size);
}
