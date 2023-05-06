#ifndef EXLANG_DEV_DISK_HPP
#define EXLANG_DEV_DISK_HPP

#include <exlang/program.hpp>
#define DISK_ACCESS_COMMAND_ADDRESS 10
#define DISK_ACCESS_DATA_ADDRESS 20

#ifdef MICRO_DEVICE

#else
#include <exlang/tinydir.h>
#include <stdio.h>

struct _file_handle {
	FILE *_fptr;
	long size;
	bool is_open;
	bool write;
};
#define NUM_FILES 16
#ifndef MAX_FILENAME_SIZE
#define MAX_FILENAME_SIZE 128 // I know that this is too small. Well eXLang is small too.
#endif

#endif

int file_system(program *p);

#endif
