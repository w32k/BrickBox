#ifndef BYTEBUF_HPP_INCLUDED
#define BYTEBUF_HPP_INCLUDED



#include <stdint.h>
#include <stddef.h>
#include <string>
#define SEGMENT_BIT 0x7F
#define CONTINUE_BIT 0x80

namespace Net {
    class Bytebuf {
        private:
            uint8_t* data;
            size_t size;
            size_t readIndex;
            size_t writeIndex;
        public:
            Bytebuf(size_t size);
            ~Bytebuf();
            void resetReadIndex();
            uint8_t readByte();
            void writeByte(uint8_t byte);
            uint16_t readU16();
            void writeU16(uint16_t value);
            uint32_t readU32();
            void writeU32(uint32_t value);
            uint64_t readU64();
            void writeU64(uint64_t value);
            int32_t readVarInt();
            void writeVarInt(int32_t value);
            int64_t readVarLong();
            void writeVarLong(int64_t value);
            void writeCString(const char* string, size_t maxLength);
            void writeString(std::string_view str, size_t maxLength);
            const uint8_t* getData();
    };
    int32_t GetSizeOfVarInt(int32_t value);
}
















#endif
