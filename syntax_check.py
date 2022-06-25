#!/usr/bin/env python3

import os
import sys

NUM = 1
NUM_OPTIONAL = 2
ADDRESS = 3
ADDRESS_OPTIONAL = 4
LABEL = 5
STR = 6
ANY = 7
NN = 8

OPTIONALS = (NUM_OPTIONAL, ADDRESS_OPTIONAL)
LABELS = []


def _validate_regular(parts: list[str]) -> bool:
    if len(parts) != 3:
        return False
    for i in range(1, 3):
        _type = get_type(parts[i])
        if _type != ADDRESS:
            return False
    return True


def _validate_single(parts: list[str]) -> bool:
    if len(parts) != 2:
        return False
    _type = get_type(parts[1])
    if _type != ADDRESS:
        return False
    return True


def _validate_jump(parts: list[str]) -> bool:
    if len(parts) < 2:
        return False
    _type = get_type(parts[1])
    if parts[1] in LABELS:
        return True
    if _type != NUM:
        return False
    if len(parts) == 3:
        _type = get_type(parts[2])
        if _type != NUM:
            return False
    return True


def _validate_goto(parts: list[str]) -> bool:
    if len(parts) != 2:
        return False
    _type = get_type(parts[1])
    return _type == NUM


def _validate_math(parts: list[str]) -> bool:
    if len(parts) != 4:
        return False
    for i in range(1, 4):
        _type = get_type(parts[i])
        if _type != ADDRESS:
            return False
    return True


def _validate_none(parts: list[str]) -> bool:
    return len(parts) == 1


def _validate_int(parts: list[str]) -> bool:
    if len(parts) != 4:
        return False
    if get_type(parts[1]) != ADDRESS:
        return False
    if get_type(parts[2]) != ADDRESS:
        return False
    if get_type(parts[3]) != NUM:
        return False
    return True


def _validate_sys(parts: list[str]) -> bool:
    if len(parts) != 2:
        return False
    if get_type(parts[1]) != NUM:
        return False
    return True


def _validate_data(parts: list[str]) -> bool:
    if len(parts) < 3:
        return False
    if get_type(parts[1]) != ADDRESS:
        return False
    return True


COMMANDS = {
    "SPRINT": _validate_single,
    "SPRINTLN": _validate_single,
    "SREADLN": _validate_regular,
    "GOTO": _validate_goto,
    "CALL": _validate_jump,
    "JE": _validate_jump,
    "JNE": _validate_jump,
    "JG": _validate_jump,
    "JGE": _validate_jump,
    "JL": _validate_jump,
    "JLE": _validate_jump,
    "CMP": _validate_regular,
    "LOG": _validate_single,
    "ADD": _validate_math,
    "SUB": _validate_math,
    "DIV": _validate_math,
    "MUL": _validate_math,
    "XOR": _validate_math,
    "OR": _validate_math,
    "AND": _validate_math,
    "POW": _validate_math,
    "SIN": _validate_regular,
    "COS": _validate_regular,
    "TAN": _validate_regular,
    "COT": _validate_regular,
    "SINH": _validate_regular,
    "COSH": _validate_regular,
    "TANH": _validate_regular,
    "COTH": _validate_regular,
    "ASIN": _validate_regular,
    "ACOS": _validate_regular,
    "ATAN": _validate_regular,
    "ACOT": _validate_regular,
    "HALT": _validate_none,
    "ALLOC": _validate_regular,
    "APPEND": _validate_regular,
    "SET": _validate_regular,
    "DEL": _validate_regular,
    "CPY": _validate_math,
    "LROTATE": _validate_regular,
    "RROTATE": _validate_regular,
    "LSHIFT": _validate_regular,
    "RSHIFT": _validate_regular,
    "SLEEP": _validate_single,
    "MILLIS": _validate_single,
    "INT": _validate_int,
    "CORE": _validate_single,
    "SYS": _validate_sys,
    "AREAD": _validate_regular,
    "AWRITE": _validate_regular,
    "PINMODE": _validate_regular,
    "DWRITE": _validate_regular,
    "DREAD": _validate_regular,
    "NOOP": _validate_none,
    "DATA": _validate_data,
    "RAND": _validate_math,
    "NUM": _validate_regular,
    "LOCATE": _validate_single,
    "PULL": _validate_regular,
    "PUSH": _validate_regular,
    "INC": _validate_single,
}


def get_type(s: str) -> int:
    if s[0] == ":":
        return LABEL
    if s[0] == "@":
        return ADDRESS
    if s[0] == '"':
        return STR
    if s[0].isnumeric():
        return NUM
    return NN


def is_num(s: str) -> bool:
    try:
        float(s)
        return True
    except ValueError:
        return False


def check_file(fname: str) -> int:
    fail = 0

    if not os.path.exists(fname):
        print(f"File not found {fname}")
        return 1

    with open(fname, "r") as f:
        data = f.read()
        lines = data.splitlines()
        lines = [line.strip() for line in lines]

        for line_number, line in enumerate(lines):
            if line.startswith("#") or len(line) < 3:
                continue
            if line.endswith(":"):
                LABELS.append(line[:-1])
                continue
            if line == "---":
                continue
            parts = line.split(" ")
            if parts[0] not in COMMANDS:
                print(f"LINE: {line_number} Command [{parts[0]}] is not known")
                fail += 1
                continue
            definition = COMMANDS[parts[0]]
            if not definition(parts):
                print(f"LINE: {line_number} ({line}) failed")
                fail += 1

    return fail


if __name__ == "__main__":
    if len(sys.argv) == 1:
        print("./syntax_check.py <filename>")
        exit(0)

    check = check_file(sys.argv[1])
    if check == 0:
        print("No issues found")
    exit(check)
