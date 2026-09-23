#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED




#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

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

typedef uintptr_t uptr;
typedef intptr_t iptr;
typedef size_t usize;
typedef size_t isize;


#endif
