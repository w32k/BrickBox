#ifndef BYTEBUF_H_INCLUDED
#define BYTEBUF_H_INCLUDED









#include <brickbox.h>
#include <net/sstring.h>


typedef struct _ByteBuf {
    u8* data;
    usize size;
    usize readIndex;
    usize writeIndex;
} ByteBuf;


BBStatus CoreCreateByteBuf(_OUT_ ByteBuf* buffer, _IN_ usize size);
BBStatus CoreDeleteByteBuf(_IN_ ByteBuf* buffer);
void CoreResetReadIndex(_IN_ ByteBuf* buffer);
BBStatus CoreReadByte(_OUT_ u8* byte, _IN_ ByteBuf* buffer);
BBStatus CoreWriteByte(_IN_ ByteBuf* buffer, _IN_ u8 byte);
BBStatus CoreReadU16(_OUT_ u16* number, _IN_ ByteBuf* buf);
BBStatus CoreWriteU16(_IN_ ByteBuf* buf, _IN_ u16 byte);
BBStatus CoreReadU32(_OUT_ u32* number, _IN_ ByteBuf* buf);
BBStatus CoreWriteU32(_IN_ ByteBuf* buf, _IN_ u32 byte);
BBStatus CoreReadU64(_OUT_ u64* number, _IN_ ByteBuf* buf);
BBStatus CoreWriteU64(_IN_ ByteBuf* buf, _IN_ u64 byte);
BBStatus CoreReadVarInt(_OUT_ i32* varInt, _IN_ ByteBuf* buf);
BBStatus CoreWriteVarInt(_IN_ ByteBuf* buf, _IN_ i32 value);
BBStatus CoreReadVarLong(_OUT_ i64* integer, _IN_ ByteBuf* buf);
BBStatus CoreWriteVarLong(_IN_ ByteBuf* buf, _IN_ i64 value);
BBStatus CoreWriteString(_IN_ ByteBuf* buf, _IN_ const char* string, _IN_ usize maxLength);
BBStatus CoreWriteSString(_IN_ ByteBuf* buf, _IN_ const SString str, _IN_ usize maxLength);
i32 CoreGetSizeOfVarInt(_IN_ i32 value);









#endif
