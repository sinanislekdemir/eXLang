#include "cmd_system.hpp"
#include "macros.hpp"
#ifdef MICRO_DEVICE
#include "bluetooth.hpp"
#include "display_ssd1306.hpp"
#include "sdcard.hpp"
#include "wifi.hpp"
#include <Arduino.h>
#endif
#include "helpers.hpp"

int command_noop(command c, program *p) {
	UNUSED(c);
	UNUSED(p);
	return 0;
}

int command_core(command c, program *p) {
	UNUSED(p);
	int core = 0;
#ifdef BOARD_ESP32
	core = xPortGetCoreID();
#endif
	if (c.variable_type[0] != TYPE_ADDRESS) {
		error_msg(ERR_STR_INVALID_TYPE, c.pid);
		return -1;
	}

	return write_area(c.variable_index[0], double(core));
}

int command_sys(command c, program *p) {
	UNUSED(p);
	int call_num = c.variable_index[0];
	if (call_num == 0) {
		return 0;
	}
#ifdef BOARD_ESP32
#ifdef WITH_WIFI
	if (call_num == 10) {
		return wifi(p);
	}
	if (call_num == 11) {
		return server(p);
	}
#endif
#ifdef WITH_BLUETOOTH_SERIAL
	if (call_num == 13) {
		return bluetooth_serial(p);
	}
#endif
#ifdef WITH_BLUETOOTH
	if (call_num == 14) {
		return bluetooth(p);
	}
#endif
#endif
#ifdef WITH_SDCARD
	if (call_num == 15) {
		return sdcard(p);
	}
#endif
#ifdef WITH_DISPLAY_SSD1306
	if (call_num == 12) {
		return display_ssd1306(p);
	}
#endif
	return 0;
};
