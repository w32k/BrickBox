#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED



#ifndef _NOSTDINC

#include <stdint.h>
#include <stdbool.h>

#define _OUT_
#define _IN_

typedef uint8_t u8;
typedef int8_t i8;

typedef uint16_t u16;
typedef int16_t i16;

typedef uint32_t u32;
typedef int32_t i32;

typedef uint64_t u64;
typedef int64_t i64;

#else


typedef unsigned char u8;
typedef char i8;

typedef unsigned short u16;
typedef short i16;

typedef unsigned int u32;
typedef int i32;

typedef unsigned long u64;
typedef long i64;

typedef u8 bool;

#endif

typedef float f32;
typedef double f64;
#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif
#ifndef NULL
#define NULL ((void*)0)
#endif

#ifdef BIT64
typedef u64 uptr;
typedef i64 iptr;
typedef u64 usize;
typedef i64 isize;
#elif BIT32
typedef u32 uptr;
typedef i32 iptr;
typedef u32 usize;
typedef i32 isize;
#else
#error "Brickbox only supports either 64bit or 32bit systems"
#endif


#endif
