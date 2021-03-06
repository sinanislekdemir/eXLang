# eXLang
Experimental Esoteric Programming Language that runs on microchips and forward.

## Learning eXLang programming

Checkout [learning.txt](https://github.com/sinanislekdemir/eXLang/blob/main/learning.txt) and [modules.txt](https://github.com/sinanislekdemir/eXLang/blob/main/modules.txt)

## Main Idea

This language is designed for micro controllers. 

- No heap allocations. All on stack. Because on embedded devices, you can not trust heap allocations. Micro-chips are not smart enough to handle memory allocations efficiently. In time, memory gets fragmanted and unreliable. Therefore, you need to preallocate the memory you need in the start-up and do not make any new `malloc` requests. All memory is calculated and handled by the compiler on stack calculations.
- Line by line compilation. RAM size on microchips are quite limited. Only a few kilobytes (8Kb on ATMega2560) and you can not afford to keep all source code in memory for advanced scope analysis or code hierarchy. We are compiling each line individually with a very limited scope called `subs`. And forget the line itself to free memory for the next line.
- No variables! Allocating variables (keeping their types and address identifiers, etc) are resource heavy operations for a microchip. Especially hard when we want to avoid `malloc` operations. Instead, we have a common memory area and we operate with memory addresses. 
- (Almost) No exception handling. Because exception handling is expensive. Instead, program just halts when things fail.
- Limited stack-trace. As we have a small footprint by design, our stack trace is also small.

With these points, an Ardunio Mega (8 kb Ram, 16Mhz Clock Speed, 8 bits) is able to run 4 different programs multithreaded. 

## Developer's Note

The code is a mess. I didn't spend any time for a better code quality. I just wanted to see it in action. I am aware that many parts of it can be rewritten in a better way. I was mostly being lazy and trying stuff. That's all. So, do not judge.

I originally designed this language for my Arduino Mega 2560 and ESP32 operating system "minik". 
But I wanted to see it in action on my Intel Based laptop.

## Interesting note (Benchmark)

In Fibonacci Sequence, exLang is 37 times faster than Python. But it's not Python to blame, Python comes with so many modern features. eXLang is a very stupid language.

exLang
```
( for i in {1..20}; do; ./exlang 01_fibonacci.exl; done; )
0.02s user 0.01s system 102% cpu 0.034 total
```

python 3.10
```
( for i in {1..20}; do; python 01_fibonacci.py > /dev/null; done; )
0.98s user 0.38s system 107% cpu 1.260 total
```

In prime number calculations:

exlang
```
./exlang 02_prime.exl  55.52s user 0.01s system 99% cpu 55.534 total
```

python
```
python 02_prime.py  169.26s user 43.72s system 100% cpu 3:32.47 total
```

## Building

You will need meson and ninja to build exLang.

```
make interpreter
sudo make install
```

## Building for Arduino Mega or ESP32 Compatible board

eXLang uses Arduino Framework + FreeRTOS for micro devices. But does not use Arduino IDE. Arduino Framework is a different thing than Arduino IDE. It is a  C++ Framework designed for micro devices. PlatformIO is a platform that supports so many micro devices and different frameworks for embedded programming and allows you to use your favorite IDE for programming.

You will need PlatformIO Core to build exlang for micro devices. You can find more information at: [https://platformio.org/install/cli](https://platformio.org/install/cli)

### Arduino Mega 2560

```
make atmega256
```

### ESP32

```
make esp32
```

