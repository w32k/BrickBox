#include <net/bytebuf.hpp>
#include <string.h>
#include <stdlib.h>
#include <net/sstring.h>

namespace Net {
    Bytebuf::Bytebuf(size_t size){
        this->data = (uint8_t*)calloc(size, sizeof(uint8_t));
        if(this->data == NULL){
            DEBUG_FAIL("failure to allocate memory of size %ld\n", size * sizeof(uint8_t));
        } else {
            this->size = size;
            this->readIndex = 0;
            this->writeIndex = 0;
        }

    }

    Bytebuf::~Bytebuf(){
        free((void*)this->data);
        this->size = 0;
        this->readIndex = 0;
        this->writeIndex = 0;
    }

    void Bytebuf::resetReadIndex(){
        this->readIndex = 0;
    }

    uint8_t Bytebuf::readByte(){
        if(this->readIndex >= this->size){
            DEBUG_FAIL("buffer overflow on %ld (max was %ld)\n", this->readIndex, this->size);
            return 0;
        } else {
            if(this->readIndex > this->writeIndex){
                DEBUG_WARNING("warning! read index (%ld) is larger then write index (%ld), getting invalid data\n", this->readIndex, this->writeIndex);
            }
            uint8_t byte = this->data[this->readIndex];
            this->readIndex++;
            return byte;
        }

    }

    void Bytebuf::writeByte(uint8_t byte){
        if(this->writeIndex >= this->size){
            DEBUG_FAIL("buffer overflow on %ld (max was %ld)\n", this->writeIndex, this->size);
        } else {
            this->data[this->writeIndex] = byte;
            this->writeIndex++;
        }
    }


    uint16_t Bytebuf::readU16(){
        if(this->readIndex + 1 >= this->size){
            DEBUG_FAIL("buffer overflow on %ld (max was %ld)\n", this->readIndex, this->size);
            return 0;
        } else {
            if(this->readIndex + 1 > this->writeIndex){
                DEBUG_WARNING("warning! read index (%ld) is larger then write index (%ld), getting invalid data\n", this->readIndex, this->writeIndex);
            }
            uint16_t value = ((uint64_t)data[this->readIndex + 1]) | ((uint64_t)data[this->readIndex] << 8);
            this->readIndex+=2;
            return value;
        }
    }

    void Bytebuf::writeU16(uint16_t value){
        if(this->writeIndex + 1 >= this->size){
            DEBUG_FAIL("buffer overflow on %ld (max was %ld)\n", this->writeIndex, this->size);
        } else {
            this->data[this->writeIndex + 1] = ((uint8_t)value);
            this->data[this->writeIndex] = ((uint8_t)(value >> 8));
            this->writeIndex+=2;
        }
    }

    uint32_t Bytebuf::readU32(){
        if(this->readIndex + 3 >= this->size){
            DEBUG_FAIL("buffer overflow on %ld (max was %ld)\n", this->readIndex, this->size);
            return 0;
        } else {
            if(this->readIndex + 3 > this->writeIndex){
                DEBUG_WARNING("warning! read index (%ld) is larger then write index (%ld), getting invalid data\n", this->readIndex, this->writeIndex);
            }
            uint32_t value = ((uint32_t)data[this->readIndex + 3]) | ((uint32_t)data[this->readIndex + 2] << 8)
                             | ((uint32_t)data[this->readIndex + 1] << 16) | ((uint32_t)data[this->readIndex] << 24);
            this->readIndex+=4;
            return value;
        }
    }

    void Bytebuf::writeU32(uint32_t value){
        if(this->writeIndex + 3 >= this->size){
            DEBUG_FAIL("buffer overflow on %ld (max was %ld)\n", this->writeIndex, this->size);
        } else {
            this->data[this->writeIndex + 3] = ((uint8_t)value);
            this->data[this->writeIndex + 2] = ((uint8_t)(value >> 8));
            this->data[this->writeIndex + 1] = ((uint8_t)(value >> 16));
            this->data[this->writeIndex] = ((uint8_t)(value >> 24));
            this->writeIndex+=4;
        }
    }

    uint64_t Bytebuf::readU64(){
        if(this->readIndex + 7 >= this->size){
            DEBUG_FAIL("buffer overflow on %ld (max was %ld)\n", this->readIndex, this->size);
            return 0;
        } else {
            if(this->readIndex + 7 > this->writeIndex){
                DEBUG_WARNING("warning! read index (%ld) is larger then write index (%ld), getting invalid data\n", this->readIndex, this->writeIndex);
            }
            uint64_t value = ((uint64_t)data[this->readIndex + 7]) | ((uint64_t)data[this->readIndex + 6] << 8)
                                | ((uint64_t)data[this->readIndex + 5] << 16) | ((uint64_t)data[this->readIndex + 4] << 24)
                                | ((uint64_t)data[this->readIndex + 3] << 32) | ((uint64_t)data[this->readIndex + 2] << 40)
                                | ((uint64_t)data[this->readIndex + 1] << 48) | ((uint64_t)data[this->readIndex + 0] << 56);
            this->readIndex+=8;
            return value;
        }
    }

    void Bytebuf::writeU64(uint64_t value){
        if(this->writeIndex + 7 >= this->size){
            DEBUG_FAIL("buffer overflow on %ld (max was %ld)\n", this->writeIndex, this->size);
        } else {
            this->data[this->writeIndex + 7] = ((uint8_t)value);
            this->data[this->writeIndex + 6] = ((uint8_t)(value >> 8));
            this->data[this->writeIndex + 5] = ((uint8_t)(value >> 16));
            this->data[this->writeIndex + 4] = ((uint8_t)(value >> 24));
            this->data[this->writeIndex + 3] = ((uint8_t)(value >> 32));
            this->data[this->writeIndex + 2] = ((uint8_t)(value >> 40));
            this->data[this->writeIndex + 1] = ((uint8_t)(value >> 48));
            this->data[this->writeIndex] = ((uint8_t)(value >> 56));
            this->writeIndex+=8;
        }
    }

    int32_t Bytebuf::readVarInt(){
        int32_t value = 0;

        for (int32_t position = 0; position < 32; position += 7) {
            uint8_t currentByte = this->readByte();
            value |= (int32_t)(currentByte & 0x7F) << position;

            if ((currentByte & 0x80) == 0){
                return value;
            }
        }
        DEBUG_WARNING("position overflow on varint, broken packet?\n");
        return 0;
    }

    void Bytebuf::writeVarInt(int32_t value){
        uint32_t uValue = value;
        while ((uValue & ~0x7F) != 0) {
            this->writeByte((uValue & 0x7F) | 0x80);
            uValue >>= 7;
        }
        this->writeByte(uValue);
    }

    int64_t Bytebuf::readVarLong(){
        int64_t value = 0;

        for (int64_t position = 0; position < 64; position += 7) {
            uint8_t currentByte = this->readByte();
            value |= (int64_t)(currentByte & 0x7F) << position;

            if ((currentByte & 0x80) == 0){
                return value;
            }
        }
        DEBUG_WARNING("position overflow on varint, broken packet?\n");
        return 0;
    }

    void Bytebuf::writeVarLong(int64_t value){
        uint64_t uValue = value;
        while ((uValue & ~0x7F) != 0) {
            this->writeByte((uValue & 0x7F) | 0x80);
            uValue >>= 7;
        }
        this->writeByte(uValue);
    }

    void Bytebuf::writeCString(const char* string, size_t maxLength){
        size_t length = strlen(string);
        if(length >= maxLength) length = maxLength;
        this->writeVarInt(length);
        for(size_t i = 0; i < length; i++){
            this->writeByte((uint8_t)string[i]);
        }
    }

    void Bytebuf::writeString(std::string_view str, size_t maxLength){
        size_t length = str.length();
        if(length >= maxLength) length = maxLength;
        this->writeVarInt(length);
        for(size_t i = 0; i < length; i++){
            this->writeByte((uint8_t)str[i]);
        }
    }
    const uint8_t* Bytebuf::getData(){
        return this->data;
    }
    int32_t GetSizeOfVarInt(int32_t value){
        int32_t size = 1;
        while((value & ~SEGMENT_BIT) != 0){
            value >>= 7;
            size++;
        }
        return size;
    }



}
