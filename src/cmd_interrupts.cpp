#include "cmd_interrupts.hpp"
#include "helpers.hpp"
#ifdef MICRO_DEVICE
#include <Arduino.h>

unsigned int _occupied_pins[64] = {0};

int command_int(command c, program *p) {
	// INT <PIN> HIGH label
	if (c.variable_type[2] != TYPE_LABEL) {
		error_msg("Sub index not found", p->pid);
		return -1;
	}

	long state = get_long(c, 1);
	int pin = int(get_long(c, 0));
	int interrupt_sub = c.variable_index[2];

	if (interrupt_sub == -1) {
		error_msg("Interrupt sub not found", c.pid);
		return -1;
	}

	p->register_interrupt(pin, state, interrupt_sub);
	return 0;
}
#endif
