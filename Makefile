# Makefile targets

.PHONY: default
default: help ;

UNAME := $(shell uname -s)

atmega2560:
	@platformio run -t upload -e mega

esp32:
	@platformio run -t upload -e esp32

interpreter:
	@mkdir build
	@cd build; cmake ../; make interpreter

clean:
	@platformio run --target clean

build-only:
	@platformio run

help:
	@echo "How to compile and install the eXperimental Language"
	@echo "make atmega2560        # Build and upload to Arduino Mega 2560 compatible board"
	@echo "make esp32             # Build and upload to ESP32 compatible board"
	@echo "make interpreter       # Build interpreter for PC"
	@echo "make clean             # Clean up PlatformIO related garbage"
	@echo "make build-only        # Build only"
