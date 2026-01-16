# Makefile targets

.PHONY: default help interpreter atmega2560 esp32 clean clean-all build-only install
default: help

UNAME := $(shell uname -s)
PREFIX ?= /usr/local

atmega2560:
	@platformio run -t upload -e mega

esp32:
	@platformio run -t upload -e esp32

interpreter:
	@mkdir -p build
	@cd build && cmake ../ && make exlang

install: interpreter
	@install -d $(PREFIX)/bin
	@install -m 755 build/src/exlang $(PREFIX)/bin/exlang
	@echo "Installed exlang to $(PREFIX)/bin/exlang"

clean:
	@platformio run --target clean

clean-all:
	@rm -rf build
	@platformio run --target clean 2>/dev/null || true
	@echo "Cleaned all build artifacts"

build-only:
	@platformio run

help:
	@echo "How to compile and install the eXperimental Language"
	@echo ""
	@echo "Targets:"
	@echo "  make atmega2560        # Build and upload to Arduino Mega 2560"
	@echo "  make esp32             # Build and upload to ESP32"
	@echo "  make interpreter       # Build interpreter for PC"
	@echo "  make install           # Install interpreter to $(PREFIX)/bin (requires sudo)"
	@echo "  make clean             # Clean PlatformIO build files"
	@echo "  make clean-all         # Clean all build files"
	@echo "  make build-only        # Build for embedded without uploading"
