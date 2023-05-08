#ifndef EXLANG_EEPROM_ACCESS_HPP
#define EXLANG_EEPROM_ACCESS_HPP

namespace exlang {
#ifdef MICRO_DEVICE

#include <Arduino.h>
#include <EEPROM.h>

static constexpr int EEPROM_SIZE = 4096;
static constexpr int EEPROM_MAJOR = 1;
static constexpr int EEPROM_MINOR = 1;

int init_eeprom();
int clean_eeprom();
int prog_count();
int eeprom_start_prog();
int eeprom_end_prog();
int eeprom_write_line(const char *buffer);
int eeprom_read_line(char *back);

#endif
}; // namespace exlang
#endif
