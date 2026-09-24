#include <net/bytebuf.h>
#include <string.h>



BBStatus NetCreateByteBuf(_OUT_ ByteBuf* buffer, _IN_ usize size){
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

BBStatus NetDeleteByteBuf(_IN_ ByteBuf* buffer){
    BBFREE(buffer->data);
    buffer->size = 0;
    buffer->readIndex = 0;
    buffer->writeIndex = 0;
    return BBSTATUS_SUCCESS;
}

void NetResetReadIndex(_IN_ ByteBuf* buffer){
    buffer->readIndex = 0;
}

BBStatus NetReadByte(_OUT_ u8* byte, _IN_ ByteBuf* buffer){
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

BBStatus NetWriteByte(_IN_ ByteBuf* buffer, _IN_ u8 byte){
    if(buffer->writeIndex >= buffer->size){
        DEBUG_FAIL("buffer overflow on %ld (max was %ld)\n", buffer->writeIndex, buffer->size);
        return BBSTATUS_BUFFER_OVERFLOW;
    }
    buffer->data[buffer->writeIndex] = byte;
    buffer->writeIndex++;
    return BBSTATUS_SUCCESS;
}


BBStatus NetReadU16(_OUT_ u16* number, _IN_ ByteBuf* buf){
    u16 firstByte, secondByte = 0;
    BBStatus status = NetReadByte((u8*)&firstByte, buf);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    status = NetReadByte((u8*)&secondByte, buf);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    *number = (u16)((firstByte << 8) | secondByte);
    return BBSTATUS_SUCCESS;
}

BBStatus NetWriteU16(_IN_ ByteBuf* buf, _IN_ u16 byte){
    u8 firstByte = (byte >> 8);
    u8 secondByte = (byte & 0xFF);
    BBStatus status = NetWriteByte(buf, firstByte);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    status = NetWriteByte(buf, secondByte);
    return status;
}

BBStatus NetReadU32(_OUT_ u32* number, _IN_ ByteBuf* buf){
    u32 firstNum, secondNum = 0;
    BBStatus status = NetReadU16((u16*)&firstNum, buf);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    status = NetReadU16((u16*)&secondNum, buf);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    *number = (u32)((firstNum << 16) | secondNum);
    return BBSTATUS_SUCCESS;
}

BBStatus NetWriteU32(_IN_ ByteBuf* buf, _IN_ u32 byte){
    u16 firstNum = (byte >> 16);
    u16 secondNum = (byte & 0xFFFF);
    BBStatus status = NetWriteU16(buf, firstNum);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    status = NetWriteU16(buf, secondNum);
    return status;
}

BBStatus NetReadU64(_OUT_ u64* number, _IN_ ByteBuf* buf){
    u64 firstNum, secondNum = 0;
    BBStatus status = NetReadU32((u32*)&firstNum, buf);
    status = NetReadU32((u32*)&secondNum, buf);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    *number = (u64)((firstNum << 32) | secondNum);
    return BBSTATUS_SUCCESS;
}

BBStatus NetWriteU64(_IN_ ByteBuf* buf, _IN_ u64 byte){
    u32 firstNum = (byte >> 32);
    u32 secondNum = (byte & 0xFFFFFFFF);
    BBStatus status = NetWriteU32(buf, firstNum);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    status = NetWriteU32(buf, secondNum);
    return status;
}

BBStatus NetReadVarInt(_OUT_ i32* integer, _IN_ ByteBuf* buf){
    i32 value = 0;

    for (i32 position = 0; position < 32; position += 7) {
        u8 currentByte = 0;
        BBStatus stat = NetReadByte(&currentByte, buf);
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

BBStatus NetWriteVarInt(_IN_ ByteBuf* buf, _IN_ i32 value){
    u32 uValue = value;
    BBStatus status = BBSTATUS_SUCCESS;
    while ((uValue & ~0x7F) != 0) {
        status = NetWriteByte(buf, (uValue & 0x7F) | 0x80);
        if(status != BBSTATUS_SUCCESS){
            return status;
        }
        uValue >>= 7;
    }
    status = NetWriteByte(buf, uValue);
    return status;
}


BBStatus NetReadVarLong(_OUT_ i64* integer, _IN_ ByteBuf* buf){
    i64 value = 0;

    for (i64 position = 0; position < 64; position += 7) {
        u8 currentByte = 0;
        BBStatus stat = NetReadByte(&currentByte, buf);
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

BBStatus NetWriteVarLong(_IN_ ByteBuf* buf, _IN_ i64 value){
    u64 uValue = value;
    BBStatus status = BBSTATUS_SUCCESS;
    while ((uValue & ~0x7F) != 0) {
        status = NetWriteByte(buf, (uValue & 0x7F) | 0x80);
        if(status != BBSTATUS_SUCCESS){
            return status;
        }
        uValue >>= 7;
    }

    status = NetWriteByte(buf, uValue);
    return status;
}

BBStatus NetWriteString(_IN_ ByteBuf* buf, _IN_ const char* string, _IN_ usize maxLength){
    usize length = strlen(string);
    if(length >= maxLength) length = maxLength;
    BBStatus status = NetWriteVarInt(buf, length);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    for(size_t i = 0; i < length; i++){
        status = NetWriteByte(buf, (uint8_t)string[i]);
        if(status != BBSTATUS_SUCCESS){
            return status;
        }
    }
    return BBSTATUS_SUCCESS;
}

BBStatus NetWriteSString(_IN_ ByteBuf* buf, _IN_ const SString str, _IN_ usize maxLength){
    usize length = str.length;
    if(length >= maxLength) length = maxLength;
    BBStatus status = NetWriteVarInt(buf, length);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    for(size_t i = 0; i < length; i++){
        status = NetWriteByte(buf, (uint8_t)str.string[i]);
        if(status != BBSTATUS_SUCCESS){
            return status;
        }
    }
    return BBSTATUS_SUCCESS;
}


i32 NetGetSizeOfVarInt(_IN_ i32 value){
    i32 size = 1;
    while((value & ~SEGMENT_BIT) != 0){
        value >>= 7;
        size++;
    }
    return size;
}
