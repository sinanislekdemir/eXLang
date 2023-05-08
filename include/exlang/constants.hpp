#ifndef EXLANG_CONSTANTS_HPP
#define EXLANG_CONSTANTS_HPP

#include <array>

namespace exlang {
static constexpr int NUM_CONSTANTS = 13;

static constexpr int TYPE_NONE = 0;
static constexpr int TYPE_STR = 1;
static constexpr int TYPE_DBL = 2;
static constexpr int TYPE_LNG = 9;
static constexpr int TYPE_CONSTANT = 3;
static constexpr int TYPE_ADDRESS_DBL = 40; // = f;
static constexpr int TYPE_ADDRESS_LNG = 41; // = l;
static constexpr int TYPE_ADDRESS_STR = 42; // = s;
static constexpr int TYPE_ADDRESS_CHR = 43; // = c;
static constexpr int TYPE_LABEL = 5;
static constexpr int TYPE_REGISTER = 6;
static constexpr int TYPE_FILE = 7;
static constexpr int TYPE_BYTE = 8;

static constexpr int STATEMENT_SPRINT = 1;
static constexpr int STATEMENT_SPRINTLN = 2;
static constexpr int STATEMENT_SREADLN = 3;
static constexpr int STATEMENT_GOTO = 4;
static constexpr int STATEMENT_CALL = 5;
static constexpr int STATEMENT_JE = 6;
static constexpr int STATEMENT_JNE = 7;
static constexpr int STATEMENT_JG = 8;
static constexpr int STATEMENT_JGE = 9;
static constexpr int STATEMENT_JL = 10;
static constexpr int STATEMENT_JLE = 11;
static constexpr int STATEMENT_CMP = 12;
static constexpr int STATEMENT_LOG = 13;
static constexpr int STATEMENT_ADD = 14;
static constexpr int STATEMENT_SUB = 15;
static constexpr int STATEMENT_DIV = 16;
static constexpr int STATEMENT_MUL = 17;
static constexpr int STATEMENT_XOR = 18;
static constexpr int STATEMENT_OR = 19;
static constexpr int STATEMENT_AND = 20;
static constexpr int STATEMENT_POW = 21;
static constexpr int STATEMENT_SIN = 22;
static constexpr int STATEMENT_COS = 23;
static constexpr int STATEMENT_TAN = 24;
static constexpr int STATEMENT_COT = 25;
static constexpr int STATEMENT_SINH = 26;
static constexpr int STATEMENT_COSH = 27;
static constexpr int STATEMENT_TANH = 28;
static constexpr int STATEMENT_COTH = 29;
static constexpr int STATEMENT_ASIN = 30;
static constexpr int STATEMENT_ACOS = 31;
static constexpr int STATEMENT_ATAN = 32;
static constexpr int STATEMENT_ACOT = 33;
static constexpr int STATEMENT_NOOP = 34;
static constexpr int STATEMENT_HALT = 38;
static constexpr int STATEMENT_ALLOC = 39;
static constexpr int STATEMENT_APPEND = 40;
static constexpr int STATEMENT_SET = 41;
static constexpr int STATEMENT_RAND = 42;
static constexpr int STATEMENT_NUM = 43;
static constexpr int STATEMENT_DEL = 44;
static constexpr int STATEMENT_CPY = 45;
static constexpr int STATEMENT_LROTATE = 46;
static constexpr int STATEMENT_RROTATE = 47;
static constexpr int STATEMENT_LSHIFT = 48;
static constexpr int STATEMENT_RSHIFT = 49;
static constexpr int STATEMENT_SLEEP = 50;
static constexpr int STATEMENT_MILLIS = 51;
static constexpr int STATEMENT_INT = 52;
static constexpr int STATEMENT_CORE = 53;
static constexpr int STATEMENT_SYS = 54;
static constexpr int STATEMENT_AREAD = 55;
static constexpr int STATEMENT_AWRITE = 56;
static constexpr int STATEMENT_PINMODE = 57;
static constexpr int STATEMENT_DWRITE = 58;
static constexpr int STATEMENT_DREAD = 59;
static constexpr int STATEMENT_DATA = 60;
static constexpr int STATEMENT_LOCATE = 61;
static constexpr int STATEMENT_PULL = 62;
static constexpr int STATEMENT_PUSH = 63;
static constexpr int STATEMENT_INC = 64;

static constexpr int LOG_SERIAL = 0;
static constexpr int LOG_FILE = 1;

#ifdef BOARD_ESP32
static constexpr int MAX_LINE_LENGTH = 128;
#else
static constexpr int MAX_LINE_LENGTH = 80;
#endif
static constexpr int PROGRAM_ERR = -1;
static constexpr int PROGRAM_FREE = 0;
static constexpr int PROGRAM_RUNNING = 1;
static constexpr int PROGRAM_HALT = 2;
static constexpr int PROGRAM_COMPILING = 3;

constexpr std::array<char, 5> MAIN = {"main"};

static constexpr int ERR_INVALID_COMMAND = 100;
static constexpr int ERR_NOT_ENOUGH_ARGUMENTS = 101;
static constexpr int ERR_INVALID_PARAMETER_TYPE = 102;
static constexpr int ERR_SOURCE_NONE = 103;
static constexpr int ERR_VARIABLE_FULL = 104;
static constexpr int ERR_DIVISION_BY_ZERO = 105;

static constexpr int ERR_COMMAND_NOT_UNDERSTOOD = 200;
static constexpr int ERR_VARIABLE_NOT_FOUND = 404;
static constexpr int ERR_ADDRESS_NOT_FOUND = 405;

static constexpr int CONST_COUNT = 13;

#ifdef MICRO_DEVICE
#include <Arduino.h>
#else
static constexpr int HIGH = 0x1;
static constexpr int LOW = 0x0;

static constexpr int INPUT = 0x0;
static constexpr int OUTPUT = 0x1;
static constexpr int INPUT_PULLUP = 0x2;

static constexpr double PI = 3.1415926535897932384626433832795;
static constexpr double HALF_PI = 1.5707963267948966192313216916398;
static constexpr double TWO_PI = 6.283185307179586476925286766559;
static constexpr double DEG_TO_RAD = 0.017453292519943295769236907684886;
static constexpr double RAD_TO_DEG = 57.295779513082320876798154814105;
static constexpr double EULER = 2.718281828459045235360287471352;

static constexpr int CHANGE = 1;
static constexpr int FALLING = 2;
static constexpr int RISING = 3;

#endif

struct constant {
	std::array<char, 16> keyword;
	double val;
	long vall;
	bool is_double;
};
}; // namespace exlang
#endif
