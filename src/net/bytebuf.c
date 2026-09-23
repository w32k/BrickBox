#include <net/bytebuf.h>
#include <string.h>



BBStatus CoreCreateByteBuf(_OUT_ ByteBuf* buffer, _IN_ usize size){
    buffer->data = BBCALLOC(size, sizeof(u8));
    if(buffer == NULL){
        DEBUG_FAIL("failure to allocate memory of size %ld\n", size * sizeof(u8));
        return BBSTATUS_OUT_OF_MEMORY;
    }
    buffer->size = size;
    buffer->readIndex = 0;
    buffer->writeIndex = 0;
    return BBSTATUS_SUCCESS;
}

BBStatus CoreDeleteByteBuf(_IN_ ByteBuf* buffer){
    BBFREE(buffer->data);
    buffer->size = 0;
    buffer->readIndex = 0;
    buffer->writeIndex = 0;
    return BBSTATUS_SUCCESS;
}

void CoreResetReadIndex(_IN_ ByteBuf* buffer){
    buffer->readIndex = 0;
}

BBStatus CoreReadByte(_OUT_ u8* byte, _IN_ ByteBuf* buffer){
    if(buffer->readIndex >= buffer->size){
        DEBUG_FAIL("buffer overflow on %ld (max was %ld)\n", buffer->readIndex, buffer->size);
        return BBSTATUS_BUFFER_OVERFLOW;
    }
    if(buffer->readIndex > buffer->writeIndex){
        DEBUG_WARNING("warning! read index (%ld) is larger then write index (%ld), getting invalid data\n", buffer->readIndex, buffer->writeIndex);
    }
    *byte = buffer->data[buffer->readIndex];
    buffer->readIndex++;
    return BBSTATUS_SUCCESS;
}

BBStatus CoreWriteByte(_IN_ ByteBuf* buffer, _IN_ u8 byte){
    if(buffer->writeIndex >= buffer->size){
        DEBUG_FAIL("buffer overflow on %ld (max was %ld)\n", buffer->writeIndex, buffer->size);
        return BBSTATUS_BUFFER_OVERFLOW;
    }
    buffer->data[buffer->writeIndex] = byte;
    buffer->writeIndex++;
    return BBSTATUS_SUCCESS;
}


BBStatus CoreReadU16(_OUT_ u16* number, _IN_ ByteBuf* buf){
    u16 firstByte, secondByte = 0;
    BBStatus status = CoreReadByte((u8*)&firstByte, buf);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    status = CoreReadByte((u8*)&secondByte, buf);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    *number = (u16)((firstByte << 8) | secondByte);
    return BBSTATUS_SUCCESS;
}

BBStatus CoreWriteU16(_IN_ ByteBuf* buf, _IN_ u16 byte){
    u8 firstByte = (byte >> 8);
    u8 secondByte = (byte & 0xFF);
    BBStatus status = CoreWriteByte(buf, firstByte);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    status = CoreWriteByte(buf, secondByte);
    return status;
}

BBStatus CoreReadU32(_OUT_ u32* number, _IN_ ByteBuf* buf){
    u32 firstNum, secondNum = 0;
    BBStatus status = CoreReadU16((u16*)&firstNum, buf);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    status = CoreReadU16((u16*)&secondNum, buf);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    *number = (u32)((firstNum << 16) | secondNum);
    return BBSTATUS_SUCCESS;
}

BBStatus CoreWriteU32(_IN_ ByteBuf* buf, _IN_ u32 byte){
    u16 firstNum = (byte >> 16);
    u16 secondNum = (byte & 0xFFFF);
    BBStatus status = CoreWriteU16(buf, firstNum);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    status = CoreWriteU16(buf, secondNum);
    return status;
}

BBStatus CoreReadU64(_OUT_ u64* number, _IN_ ByteBuf* buf){
    u64 firstNum, secondNum = 0;
    BBStatus status = CoreReadU32((u32*)&firstNum, buf);
    status = CoreReadU32((u32*)&secondNum, buf);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    *number = (u64)((firstNum << 32) | secondNum);
    return BBSTATUS_SUCCESS;
}

BBStatus CoreWriteU64(_IN_ ByteBuf* buf, _IN_ u64 byte){
    u32 firstNum = (byte >> 32);
    u32 secondNum = (byte & 0xFFFFFFFF);
    BBStatus status = CoreWriteU32(buf, firstNum);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    status = CoreWriteU32(buf, secondNum);
    return status;
}

BBStatus CoreReadVarInt(_OUT_ i32* integer, _IN_ ByteBuf* buf){
    i32 value = 0;

    for (i32 position = 0; position < 32; position += 7) {
        u8 currentByte = 0;
        BBStatus stat = CoreReadByte(&currentByte, buf);
        if(stat != BBSTATUS_SUCCESS){
            return stat;
        }
        value |= (i32)(currentByte & 0x7F) << position;

        if ((currentByte & 0x80) == 0){
            *integer = value;
            return BBSTATUS_SUCCESS;
        }
    }
    return BBSTATUS_POSITION_OVERFLOW;
}

BBStatus CoreWriteVarInt(_IN_ ByteBuf* buf, _IN_ i32 value){
    u32 uValue = value;
    BBStatus status = BBSTATUS_SUCCESS;
    while ((uValue & ~0x7F) != 0) {
        status = CoreWriteByte(buf, (uValue & 0x7F) | 0x80);
        if(status != BBSTATUS_SUCCESS){
            return status;
        }
        uValue >>= 7;
    }

    status = CoreWriteByte(buf, uValue);
    return status;
}

BBStatus CoreReadVarLong(_OUT_ i64* integer, _IN_ ByteBuf* buf){
    i64 value = 0;

    for (i64 position = 0; position < 64; position += 7) {
        u8 currentByte = 0;
        BBStatus stat = CoreReadByte(&currentByte, buf);
        if(stat != BBSTATUS_SUCCESS){
            return stat;
        }
        value |= (i64)(currentByte & 0x7F) << position;

        if ((currentByte & 0x80) == 0){
            *integer = value;
            return BBSTATUS_SUCCESS;
        }
    }
    return BBSTATUS_POSITION_OVERFLOW;
}

BBStatus CoreWriteVarLong(_IN_ ByteBuf* buf, _IN_ i64 value){
    u64 uValue = value;
    BBStatus status = BBSTATUS_SUCCESS;
    while ((uValue & ~0x7F) != 0) {
        status = CoreWriteByte(buf, (uValue & 0x7F) | 0x80);
        if(status != BBSTATUS_SUCCESS){
            return status;
        }
        uValue >>= 7;
    }

    status = CoreWriteByte(buf, uValue);
    return status;
}

BBStatus CoreWriteString(_IN_ ByteBuf* buf, _IN_ const char* string, _IN_ usize maxLength){
    usize length = strlen(string);
    if(length >= maxLength) length = maxLength;
    BBStatus status = CoreWriteVarInt(buf, length);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    for(size_t i = 0; i < length; i++){
        status = CoreWriteByte(buf, (uint8_t)string[i]);
        if(status != BBSTATUS_SUCCESS){
            return status;
        }
    }
    return BBSTATUS_SUCCESS;
}

BBStatus CoreWriteSString(_IN_ ByteBuf* buf, _IN_ const SString str, _IN_ usize maxLength){
    usize length = str.length;
    if(length >= maxLength) length = maxLength;
    BBStatus status = CoreWriteVarInt(buf, length);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    for(size_t i = 0; i < length; i++){
        status = CoreWriteByte(buf, (uint8_t)str.string[i]);
        if(status != BBSTATUS_SUCCESS){
            return status;
        }
    }
    return BBSTATUS_SUCCESS;
}


i32 CoreGetSizeOfVarInt(_IN_ i32 value){
    i32 size = 1;
    while((value & ~SEGMENT_BIT) != 0){
        value >>= 7;
        size++;
    }
    return size;
}
