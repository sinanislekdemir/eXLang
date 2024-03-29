#ifdef MICRO_DEVICE
#include <Arduino.h>
#else
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#endif

#include "exlang/constants.hpp"

#ifndef MICRO_DEVICE
#include <sys/time.h>
#endif
long long _millis() {
#ifdef MICRO_DEVICE
	return millis();
#else
	struct timeval te;
	gettimeofday(&te, nullptr);
	long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
	return milliseconds;
#endif
}

int pfree() {
#ifdef BOARD_ATMEGA
	extern int __heap_start, *__brkval;
	int v;
	int x = (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
	return x;
#endif

#ifdef BOARD_ESP32
	return ESP.getFreeHeap();
#endif
	return 0;
}

unsigned int extract_size(const char *text, char delimiter, unsigned int index) {
	unsigned int index_counter = 0;
	bool quote_block = false;
	unsigned int result = 0;

	for (unsigned int i = 0; i < strlen(text); i++) {
		if (index_counter == index) {
			for (unsigned int j = i; j < strlen(text); j++) {
				if (text[j] == '"') {
					quote_block = !quote_block;
				}
				if (text[j] == delimiter) {
					if (quote_block) {
						result++;
						continue;
					} else {
						return result;
					}
				}
				result++;
			}
			return result;
		}
		if (text[i] == '"') {
			quote_block = !quote_block;
		}
		if (text[i] == delimiter && !quote_block) {
			index_counter++;
		}
	}
	return result;
}

// strtok should do fine but I need to keep "..." intact
int extract(const char *text, char delimiter, unsigned int index, char *back) {
	// Extract the given parameter from a full string and return *back

	int cursor = 0;
	unsigned int index_counter = 0;
	bool quote_block = false;

	for (unsigned int i = 0; i < strlen(text); i++) {
		if (index_counter == index) {
			for (unsigned int j = i; j < strlen(text); j++) {
				if (text[j] == '"') {
					quote_block = !quote_block;
				}
				if (text[j] == delimiter) {
					if (quote_block) {
						back[cursor++] = text[j];
						continue;
					} else {
						return strlen(back);
					}
				}
				back[cursor++] = text[j];
			}
			return strlen(back);
		}
		if (text[i] == '"') {
			quote_block = !quote_block;
		}
		if (text[i] == delimiter && !quote_block) {
			index_counter++;
		}
	}
	return strlen(back);
}

unsigned int argc(const char *text, char delimiter) {
	// return argument count given in a text
	bool string_literal = false;
	unsigned int count = 1;
	for (unsigned int i = 0; i < strlen(text); i++) {
		if (text[i] == '"') {
			string_literal = !string_literal;
		}
		if (!string_literal && text[i] == delimiter) {
			count += 1;
		}
	}
	return count;
}

unsigned int arg_loc(const char *arg) {
	char tmp[MAX_LINE_LENGTH] = {0};
	strcpy(tmp, arg + 1);
	return atoi(tmp);
}

bool is_address_type(unsigned int t) { return t >= 40 && t <= 43; }

short arg_type(const char *arg) {
	// return the argument's type
	int li = strlen(arg) - 1;
	if (li == -1) {
		return TYPE_NONE;
	}
	if (arg[li] == ':') {
		return TYPE_LABEL;
	}
	if (strncmp(arg, "0x", 2) == 0) {
		return TYPE_BYTE;
	}
	if (arg[0] == '"')
		return TYPE_STR;
	if (arg[li] == 'f')
		return TYPE_ADDRESS_DBL;
	if (arg[li] == 'l')
		return TYPE_ADDRESS_LNG;
	if (arg[li] == 's')
		return TYPE_ADDRESS_STR;
	if (arg[li] == 'c')
		return TYPE_ADDRESS_CHR;

	bool isnum = true;
	bool isdbl = false;
	for (unsigned int i = 0; i < strlen(arg); i++) {
		if (!isdigit(arg[i]) && arg[i] != '.' && arg[i] != '-') {
			isnum = false;
			break;
		}
		if (arg[i] == '.') {
			isdbl = true;
		}
	}
	if (isnum) {
		if (isdbl)
			return TYPE_DBL;
		return TYPE_LNG;
	}

	// skip file check for now.
	// @TODO: Do File type check once we have the
	//        file system drivers in-place.
	return TYPE_LABEL;
}

double ctod(char *data) {
	double resp = 0;
	memcpy(&resp, data, sizeof(resp));
	return resp;
}

char hex2c(char *hex) {
	char decimal = 0;
	char base = 1;
	for (unsigned int i = 3; i >= 2; i--) {
		if (hex[i] >= 0 && hex[i] <= 9) {
			decimal += (hex[i] - 48) * base;
			base *= 16;
		} else if (hex[i] >= 'A' && hex[i] <= 'F') {
			decimal += (hex[i] - 55) * base;
			base *= 16;
		} else if (hex[i] >= 'a' && hex[i] <= 'f') {
			decimal += (hex[i] - 87) * base;
			base *= 16;
		}
	}
	return decimal;
}

void ltrim(char *src) {
	char *dst;

	/* find position of first non-space character */
	for (dst = src; *src == ' '; src++) {
		;
	}

	/* nothing to do */
	if (dst == src)
		return;

	/* K&R style strcpy() */
	while ((*dst++ = *src++)) {
		;
	}

	return;
}

bool is_data_type(int type) {
	return (type == TYPE_LNG || type == TYPE_DBL || type == TYPE_STR || type == TYPE_FILE || type == TYPE_REGISTER ||
		type == TYPE_CONSTANT || type == TYPE_BYTE || type >= TYPE_ADDRESS_DBL);
}
