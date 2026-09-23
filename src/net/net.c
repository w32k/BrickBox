#include <net/net.h>
#include <extern/miniz.h>


BBStatus NetSetupPacket(_IN_ bool compressionEnabled, _IN_ usize size, _OUT_ ByteBuf* packet){
    if(compressionEnabled == FALSE){
        BBStatus status = NetCreateByteBuf(packet, size + NetGetSizeOfVarInt(size));
        if(status != BBSTATUS_SUCCESS){
            return status;
        }

        status = NetWriteVarInt(packet, size);
        if(status != BBSTATUS_SUCCESS) {
            NetDeleteByteBuf(packet);
            return BBSTATUS_CANT_WRITE_INTO_BYTEBUF;
        }
    } else {
        size += NetGetSizeOfVarInt(0x0);
        BBStatus status = NetCreateByteBuf(packet, size + NetGetSizeOfVarInt(size));
        if(status != BBSTATUS_SUCCESS){
            return status;
        }
        status = NetWriteVarInt(packet, size);
        if(status != BBSTATUS_SUCCESS) {
            NetDeleteByteBuf(packet);
            return BBSTATUS_CANT_WRITE_INTO_BYTEBUF;
        }
        status = NetWriteVarInt(packet, 0x00);
        if(status != BBSTATUS_SUCCESS) {
            NetDeleteByteBuf(packet);
            return BBSTATUS_CANT_WRITE_INTO_BYTEBUF;
        }
    }
    return BBSTATUS_SUCCESS;
}

BBStatus NetDecompressPacket(_IN_ ByteBuf* iPacket, _IN_ usize pSize, _IN_ usize dSize, _OUT_ ByteBuf* rPacket){
    BBStatus status = NetCreateByteBuf(rPacket, dSize);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    int zResult = uncompress(rPacket->data, (unsigned long*)&dSize, iPacket->data, pSize);
    if(zResult != Z_OK){
        switch(zResult){
            case Z_BUF_ERROR:{
                DEBUG_FAIL("Buffer not large enough\n");
                break;
            }
            case Z_MEM_ERROR:{
                DEBUG_FAIL("Out of memory\n");
                break;
            }
            case Z_DATA_ERROR:{
                DEBUG_FAIL("Data was corrupted\n");
                break;
            }
        }
        NetDeleteByteBuf(rPacket);
        return BBSTATUS_CANT_DECOMPRESS_PACKET;
    }
    return BBSTATUS_SUCCESS;
}
