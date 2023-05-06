#include <exlang/dev_disk.hpp>

#include <exlang/memory.hpp>

#ifndef MICRO_DEVICE

#include <stdio.h>
tinydir_dir _dir;
bool _dir_open = false;
bool _init_fs = false;

_file_handle _ex_files[NUM_FILES]; // I guess 16 file handlers are more than enough;

int file_system(program *p) {
	if (!_init_fs) {
		for (unsigned int i = 0; i < NUM_FILES; i++) {
			_ex_files[i].is_open = false;
			_ex_files[i].write = false;
			_ex_files[i].size = 0;
		}
		_init_fs = true;
	}
	long cmd_var = read_area_long(DISK_ACCESS_COMMAND_ADDRESS);
	switch (cmd_var) {
	case 0: {
		error_msg("DISK_ACCESS_COMMAND_ADDRESS not defined", p->pid);
		return -1;
		break;
	}
	case 1: {
		if (_dir_open) {
			tinydir_close(&_dir);
			_dir_open = false;
		}
		char path[MAX_FILENAME_SIZE] = {0};
		read_area_str(DISK_ACCESS_DATA_ADDRESS, MAX_FILENAME_SIZE, path);
		int check = tinydir_open_sorted(&_dir, path);
		if (check == -1) {
			_dir_open = false;
			return -1;
		}
		_dir_open = true;
		free_area(DISK_ACCESS_DATA_ADDRESS, MAX_FILENAME_SIZE);
		write_area(DISK_ACCESS_DATA_ADDRESS, double(_dir.n_files));
		return 0;
	}
	case 2: {
		if (!_dir_open) {
			error_msg("No open directory", p->pid);
			return -1;
		}
		size_t file_index = size_t(read_area_long(DISK_ACCESS_DATA_ADDRESS));
		if (file_index >= _dir.n_files) {
			error_msg("Index out of bounds", p->pid);
			return -1;
		}
		tinydir_file file;
		int check = tinydir_readfile_n(&_dir, &file, file_index);
		if (check == -1) {
			error_msg("Error getting the file", p->pid);
			return -1;
		}
		free_area(DISK_ACCESS_DATA_ADDRESS, MAX_FILENAME_SIZE + 10);
		write_area(DISK_ACCESS_DATA_ADDRESS, long(file.is_dir));
		write_area(DISK_ACCESS_DATA_ADDRESS + 10, file.name);
		return 0;
	}
	case 3: {
		tinydir_close(&_dir);
		_dir_open = false;
		return 0;
	}
	case 4: { // Open for read
		char fname[256] = {0};
		read_area_str(DISK_ACCESS_DATA_ADDRESS, 256, fname);
		for (unsigned int i = 0; i < NUM_FILES; i++) {
			if (_ex_files[i].is_open) {
				continue;
			}
			if ((_ex_files[i]._fptr = fopen(fname, "rb")) == NULL) {
				error_msg("Error opening file", p->pid);
				return -1;
				_ex_files[i].is_open = false;
			}
			_ex_files[i].is_open = true;
			_ex_files[i].write = false;
			fseek(_ex_files[i]._fptr, 0, SEEK_END);
			_ex_files[i].size = ftell(_ex_files[i]._fptr);
			fseek(_ex_files[i]._fptr, 0, SEEK_SET);
			free_area(DISK_ACCESS_DATA_ADDRESS, MAX_FILENAME_SIZE);
			write_area(DISK_ACCESS_DATA_ADDRESS, long(i));
			write_area(DISK_ACCESS_DATA_ADDRESS + 10, long(_ex_files[i].size));
			return 0;
		}
		error_msg("Too many open files", p->pid);
		return -1;
	}
	case 5: { // Open for write
		char fname[256] = {0};
		read_area_str(DISK_ACCESS_DATA_ADDRESS, 256, fname);
		for (unsigned int i = 0; i < NUM_FILES; i++) {
			if (_ex_files[i].is_open) {
				continue;
			}
			if ((_ex_files[i]._fptr = fopen(fname, "wb")) == NULL) {
				error_msg("Error opening file", p->pid);
				return -1;
				_ex_files[i].is_open = false;
			}
			_ex_files[i].is_open = true;
			_ex_files[i].write = true;
			fseek(_ex_files[i]._fptr, 0, SEEK_END);
			_ex_files[i].size = ftell(_ex_files[i]._fptr);
			fseek(_ex_files[i]._fptr, 0, SEEK_SET);
			free_area(DISK_ACCESS_DATA_ADDRESS, MAX_FILENAME_SIZE);
			write_area(DISK_ACCESS_DATA_ADDRESS, long(i));
			write_area(DISK_ACCESS_DATA_ADDRESS + 10, long(_ex_files[i].size));
			return 0;
		}
		error_msg("Too many open files", p->pid);
		return -1;
	}
	case 6: { // Set cursor
		int index = int(read_area_long(DISK_ACCESS_DATA_ADDRESS));
		long cloc = long(read_area_long(DISK_ACCESS_DATA_ADDRESS + 10));
		if (index < 0 || index >= NUM_FILES) {
			error_msg("Invalid file index", p->pid);
			return -1;
		}
		if (!_ex_files[index].is_open) {
			error_msg("File is not open", p->pid);
			return -1;
		}
		if (cloc >= _ex_files[index].size) {
			error_msg("File cursor is out of bounds", p->pid);
			return -1;
		}
		fseek(_ex_files[index]._fptr, cloc, SEEK_SET);
		return 0;
	}
	case 7: { // Read
		int index = int(read_area_long(DISK_ACCESS_DATA_ADDRESS));
		size_t size = size_t(read_area_long(DISK_ACCESS_DATA_ADDRESS + 10));
		if (index < 0 || index >= NUM_FILES) {
			error_msg("Invalid file index", p->pid);
			return -1;
		}
		if (!_ex_files[index].is_open) {
			error_msg("File is not open", p->pid);
			return -1;
		}
		if (_ex_files[index].write) {
			error_msg("File is open for write", p->pid);
			return -1;
		}
		if (size > MAX_LINE_LENGTH) {
			error_msg("Max buffer size can be MAX_LINE_LENGTH", p->pid);
			return -1;
		}
		char buffer[MAX_LINE_LENGTH] = {0};
		size_t eof = fread(buffer, size, 1, _ex_files[index]._fptr);
		free_area(DISK_ACCESS_DATA_ADDRESS, MAX_LINE_LENGTH);
		write_area(DISK_ACCESS_DATA_ADDRESS, long(eof));
		write_area(DISK_ACCESS_DATA_ADDRESS + 10, buffer);
		return 0;
	}
	case 8: { // Write
		int index = int(read_area_long(DISK_ACCESS_DATA_ADDRESS));
		size_t size = size_t(read_area_long(DISK_ACCESS_DATA_ADDRESS + 10));
		if (index < 0 || index >= NUM_FILES) {
			error_msg("Invalid file index", p->pid);
			return -1;
		}
		if (!_ex_files[index].is_open) {
			error_msg("File is not open", p->pid);
			return -1;
		}
		if (!_ex_files[index].write) {
			error_msg("File is open for read", p->pid);
			return -1;
		}
		if (size > MAX_LINE_LENGTH) {
			error_msg("Max buffer size can be MAX_LINE_LENGTH", p->pid);
			return -1;
		}
		char buffer[MAX_LINE_LENGTH] = {0};
		read_area_str(DISK_ACCESS_DATA_ADDRESS + 20, MAX_LINE_LENGTH, buffer);

		fwrite(buffer, size, 1, _ex_files[index]._fptr);
		return 0;
	}
	case 9: { // Close
		int index = int(read_area_long(DISK_ACCESS_DATA_ADDRESS));
		if (index < 0 || index >= NUM_FILES) {
			error_msg("Invalid file index", p->pid);
			return -1;
		}
		if (!_ex_files[index].is_open) {
			error_msg("File is not open", p->pid);
			return -1;
		}
		fclose(_ex_files[index]._fptr);
		_ex_files[index].is_open = false;
		_ex_files[index].size = 0;
		_ex_files[index].write = false;
		return 0;
	}
	default: {
		error_msg("Unknown command", p->pid);
		return -1;
	}
	}
}
#else
int file_system(program *p) { return -1; }
#endif
