#ifndef EXLANG_ERROR_HPP
#define EXLANG_ERROR_HPP

namespace exlang {
struct error {
	unsigned int code;
	unsigned int pid;
};

error raise(unsigned int pid, unsigned int code);
}; // namespace exlang
#endif
