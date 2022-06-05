#ifndef _cmd_analog_hpp
#define _cmd_analog_hpp

#include "program.hpp"

#ifdef MICRO_DEVICE
int command_analogread(command c, program *p);
int command_analogwrite(command c, program *p);
int command_analogref(command c, program *p);
#endif

#endif
