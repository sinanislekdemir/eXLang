#include "exlang/cmd_log.hpp"
#include "exlang/macros.hpp"

int output = LOG_SERIAL;

void set_output(int output_type) { output = output_type; }
/**
 * @brief We need a better logging mechanism here
 *
 * @param c
 * @param p
 * @return int
 */
int command_log(command c, program *p) {
	UNUSED(c);
	UNUSED(p);
	return 0;
}
