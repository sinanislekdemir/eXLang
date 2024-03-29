#include "exlang/cmd_time.hpp"
#include "exlang/helpers.hpp"
#include "exlang/macros.hpp"

int command_sleep(command c, program *_p) {
	if (c.arg_count < 1) {
		error_msg(ERR_STR_NOT_ENOUGH_PARAMS, c.pid);
		return -1;
	}
	long duration = get_long(c, 0);
	_p->_sleep = true;
	_p->_sleep_duration = (unsigned long)(duration);

	_p->_sleep_start = _millis();
	return 0;
}

int command_millis(command c, program *_p) {
	UNUSED(_p);
	long mls = _millis();
	write_area(c.variable_index[0], mls);
	return 0;
}
