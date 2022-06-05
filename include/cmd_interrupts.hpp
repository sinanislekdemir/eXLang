#ifndef _cmd_interrupts_hpp
#define _cmd_interrupts_hpp

#include "program.hpp"

#ifdef MICRO_DEVICe
int command_int(command c, program *p);
#endif
#endif
