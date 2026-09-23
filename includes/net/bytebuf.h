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


BBStatus NetCreateByteBuf(_OUT_ ByteBuf* buffer, _IN_ usize size);
BBStatus NetDeleteByteBuf(_IN_ ByteBuf* buffer);
void NetResetReadIndex(_IN_ ByteBuf* buffer);
BBStatus NetReadByte(_OUT_ u8* byte, _IN_ ByteBuf* buffer);
BBStatus NetWriteByte(_IN_ ByteBuf* buffer, _IN_ u8 byte);
BBStatus NetReadU16(_OUT_ u16* number, _IN_ ByteBuf* buf);
BBStatus NetWriteU16(_IN_ ByteBuf* buf, _IN_ u16 byte);
BBStatus NetReadU32(_OUT_ u32* number, _IN_ ByteBuf* buf);
BBStatus NetWriteU32(_IN_ ByteBuf* buf, _IN_ u32 byte);
BBStatus NetReadU64(_OUT_ u64* number, _IN_ ByteBuf* buf);
BBStatus NetWriteU64(_IN_ ByteBuf* buf, _IN_ u64 byte);
BBStatus NetReadVarInt(_OUT_ i32* varInt, _IN_ ByteBuf* buf);
BBStatus NetWriteVarInt(_IN_ ByteBuf* buf, _IN_ i32 value);
BBStatus NetReadVarLong(_OUT_ i64* integer, _IN_ ByteBuf* buf);
BBStatus NetWriteVarLong(_IN_ ByteBuf* buf, _IN_ i64 value);
BBStatus NetWriteString(_IN_ ByteBuf* buf, _IN_ const char* string, _IN_ usize maxLength);
BBStatus NetWriteSString(_IN_ ByteBuf* buf, _IN_ const SString str, _IN_ usize maxLength);
i32 NetGetSizeOfVarInt(_IN_ i32 value);









#endif
