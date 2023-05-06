#ifndef EXLANG_ERROR_HPP
#define EXLANG_ERROR_HPP

struct error {
	unsigned int code;
	unsigned int pid;
};

error raise(unsigned int pid, unsigned int code);
#endif
