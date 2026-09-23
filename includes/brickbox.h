#ifndef BRICKBOX_H_INCLUDED
#define BRICKBOX_H_INCLUDED








#include <types.h>
#ifndef _NOSTDINC
#include <stdio.h>
#include <stdlib.h>
#endif

#define VER "DEV"
#define MC_VER "Minecraft 1.21.11"
#define PROT_VER 774
#define SEGMENT_BIT 0x7F
#define CONTINUE_BIT 0x80

#define BBCALLOC(amount, size) calloc(amount, size)
#define BBMALLOC(amount) malloc(amount)
#define BBFREE(address) free(address)

#define DEBUG_FAIL(message, ...) printf("[FAIL] (%s:%d): " message, __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG_PASS(message, ...) printf("[PASS] (%s:%d): " message, __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG_WARNING(message, ...) printf("[WARNING] (%s:%d): " message, __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG_INFO(message, ...) printf("[INFO] (%s:%d): " message, __FILE__, __LINE__, ##__VA_ARGS__)

#define ARR_LEN(x) (sizeof(x) / sizeof(x[0]))

typedef enum _BBStatus {
    BBSTATUS_SUCCESS,
    BBSTATUS_FAILED,
    BBSTATUS_WOULD_BLOCK,
    BBSTATUS_OUT_OF_MEMORY,
    BBSTATUS_COULDNT_CREATE_SOCKET,
    BBSTATUS_IP_CONV_FAILED,
    BBSTATUS_CONNECTION_FAILED,
    BBSTATUS_FAILED_TO_READ,
    BBSTATUS_CANT_CREATE_BYTEBUF,
    BBSTATUS_CANT_DECOMPRESS_PACKET,
    BBSTATUS_NOT_IMPLEMENTED,
    BBSTATUS_CANT_SEND,
    BBSTATUS_POSITION_OVERFLOW,
    BBSTATUS_CONNECTION_ENDED,
    BBSTATUS_COULDNT_CHECK,
    BBSTATUS_NO_EVENT_AVALIABLE,
    BBSTATUS_CANT_WRITE_INTO_BYTEBUF,
    BBSTATUS_BUFFER_OVERFLOW,
    BBSTATUS_CANT_START_WINDOWING_PLATFORM,
    BBSTATUS_CANT_CREATE_WINDOW,
    BBSTATUS_CANT_CREATE_GRAPHICS_CONTEXT,
    BBSTATUS_CANT_SETUP_GRAPHICS_API
} BBStatus;












#endif
