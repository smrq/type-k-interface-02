#pragma once

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <util/twi.h>
#include <stddef.h>
#include <stdbool.h>

#define local static

#define PACKED __attribute__((__packed__))
#define UNUSED __attribute__ ((unused))

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;

#define MEMORY_BARRIER __asm__ __volatile__("" ::: "memory");

#define ENABLE_GLOBAL_INTERRUPTS()  do { MEMORY_BARRIER; sei(); MEMORY_BARRIER; } while (0)
#define DISABLE_GLOBAL_INTERRUPTS() do { MEMORY_BARRIER; cli(); MEMORY_BARRIER; } while (0)
