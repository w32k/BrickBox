#include <net/client.h>
#include <net/net.h>
#include <net/packets/clientbound.h>



// TODO: rewrite this
// its a mess lol

const PacketHandlerCallback loginStateCallbacks[] = {
    [0] = NetLoginDisconnect, [1] = NetHello, [2] = NetLoginFinished, [3] = NetInitCompression,
    [4] = NetCustomQuery, [5] = NetCookieRequest
};

const PacketHandlerCallback configStateCallbacks[] = {
    [0] = NetCookieRequest, [1] = NetCustomPayload, [2] = NetConfigDisconnect, [3] = NetFinishConfiguration,
    [4] = NetKeepAlive, [5] = NetPing, [6] = NetResetChat, [7] = NetRegistryData, [8] = NetResourcePackPop,
    [9] = NetResourcePackPush, [10] = NetStoreCookie, [11] = NetTransfer, [12] = NetUpdateEnabledFeatures,
    [13] = NetUpdateTags, [14] = NetSelectUnknownPacks, [15] = NetCustomReportDetails, [16] = NetServerLinks,
    [17] = NetClearDialog, [18] = NetShowDialog, [19] = NetCodeOfConduct
};


static inline BBStatus ReadUncompressedPacket(_IN_ NetClient* client, _IN_ ByteBuf* packet, _IN_ i32 length){
    //DEBUG_PASS("parsing uncompressed packet (%d)\n", length);
    BBStatus status = NetCreateByteBuf(packet, length);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    status = NetClientRead(client, length, packet->data);
    if(status != BBSTATUS_SUCCESS){
        NetDeleteByteBuf(packet);
        return status;
    }
    packet->writeIndex = length;
    return BBSTATUS_SUCCESS;
}

static inline BBStatus ReadCompressedPacket(_IN_ NetClient* client, _IN_ i32 packetLength,
                            _IN_ i32 dataLength, _OUT_ ByteBuf* packet){
    //DEBUG_PASS("parsing compressed packet (p: %d, d: %d)\n", packetLength, dataLength);
    ByteBuf compressedPacket = {0};
    BBStatus status = NetCreateByteBuf(&compressedPacket, packetLength);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    status = NetClientRead(client, packetLength, compressedPacket.data);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    status = NetDecompressPacket(&compressedPacket, packetLength, dataLength, packet);
    if(status != BBSTATUS_SUCCESS){
        NetDeleteByteBuf(&compressedPacket);
        return status;
    }
    packet->writeIndex = dataLength;
    NetDeleteByteBuf(&compressedPacket);
    return BBSTATUS_SUCCESS;
}

BBStatus NetClientHandleEvents(_IN_ NetClient* client){
    ByteBuf packet = {0};
    i32 length = 0;
    BBStatus status = NetClientReadVarInt(client, &length);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    if(length <= 0 || length >= 2000000) return BBSTATUS_POSITION_OVERFLOW;
    if(client->compressionEnabled == FALSE){
        status = ReadUncompressedPacket(client, &packet, length);
        if(status != BBSTATUS_SUCCESS){
            return status;
        }
    } else {
        i32 dataLength = 0;
        status = NetClientReadVarInt(client, &dataLength);
        if(status != BBSTATUS_SUCCESS){
            return status;
        }
        if(NetGetSizeOfVarInt(dataLength) > length) {
            return BBSTATUS_SUCCESS;
        }
        i32 packetLength = length - NetGetSizeOfVarInt(dataLength);
        if(dataLength == 0){
            //DEBUG_PASS("parsing uncompressed packet in compressed format\n");
            status = ReadUncompressedPacket(client, &packet, packetLength);
            if(status != BBSTATUS_SUCCESS){
                return status;
            }
        } else if(dataLength > 0){
            status = ReadCompressedPacket(client, packetLength, dataLength, &packet);
            if(status != BBSTATUS_SUCCESS){
                return status;
            }
        }
    }

    i32 id = 0;
    status = NetReadVarInt(&id, &packet);
    if(status != BBSTATUS_SUCCESS){
        DEBUG_FAIL("cant read\n");
        goto FAIL;
    }
    //DEBUG_INFO("id: %d, state: %d, length: %d\n", id, client->state, length);

    switch(client->state){
        case NCSTATE_LOGIN:{
            usize len = ARR_LEN(loginStateCallbacks);
            if(id >= len || id < 0){
               DEBUG_FAIL("invalid packet id! %d (at state %d)\n", id, client->state);
            } else {
                status = loginStateCallbacks[id](client, &packet);
            }
            break;
        }
        case NCSTATE_CONFIG:{
            usize len = ARR_LEN(configStateCallbacks);
            if(id >= len || id < 0){
                DEBUG_FAIL("invalid packet id! %d (at state %d)\n", id, client->state);
            } else {
                status = configStateCallbacks[id](client, &packet);
            }
            break;
        }
        case NCSTATE_PLAY:{
            if(id == 43) {
                NetKeepAlive(client, &packet);
            }
            status = BBSTATUS_NOT_IMPLEMENTED;
            break;
        }
    }
FAIL:
    NetDeleteByteBuf(&packet);
    return status;
}
