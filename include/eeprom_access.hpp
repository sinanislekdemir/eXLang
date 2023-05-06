#ifndef _eeprom_access_hpp
#define _eeprom_access_hpp

#ifdef MICRO_DEVICE

#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_SIZE 4096
#define EEPROM_MAJOR 1
#define EEPROM_MINOR 1

int init_eeprom();
int clean_eeprom();
int prog_count();
int eeprom_start_prog();
int eeprom_end_prog();
int eeprom_write_line(const char *buffer);
int eeprom_read_line(char *back);

#endif

#endif
