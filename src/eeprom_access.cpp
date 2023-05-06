#include "exlang/eeprom_access.hpp"

#ifdef MICRO_DEVICE
int init_eeprom() {
#ifdef BOARD_ESP32
	EEPROM.begin(EEPROM_SIZE);
#else
	EEPROM.begin();
#endif
	if ((EEPROM.read(0) != EEPROM_MAJOR) || EEPROM.read(1) != EEPROM_MINOR) {
		clean_eeprom();
	}
}
int clean_eeprom() {
	char buffer[128] = {0};
	for (unsigned int i = 0; i < 32; i++) {
		EEPROM.put(i * 128, &buffer);
#ifdef BOARD_ESP32
		EEPROM.commit();
		vTaskDelay(1);
#endif
	}
	EEPROM.write(0, EEPROM_MAJOR);
	EEPROM.write(1, EEPROM_MINOR);
	return 0;
}
int prog_count() {}
int eeprom_start_prog() {}
int eeprom_end_prog() {}
int eeprom_write_line(const char *buffer);
int eeprom_read_line(char *back);
#endif
