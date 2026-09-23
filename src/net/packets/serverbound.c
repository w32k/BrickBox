#include <net/packets/clientbound.h>
#include <net/bytebuf.h>
#include <string.h>
#include <net/sstring.h>
#include <net/net.h>



BBStatus NetHandshakePacket(_IN_ NetClient* client, _IN_ const char* ip, _IN_ u16 port){
    ByteBuf packet = {0};
    usize ipLength = strlen(ip);
    if(ipLength >= 255) ipLength = 255;
    usize size = CoreGetSizeOfVarInt(0x00) + CoreGetSizeOfVarInt(PROT_VER) +
                CoreGetSizeOfVarInt(ipLength) + ipLength + sizeof(u16) + CoreGetSizeOfVarInt(2);
    // BBStatus status = CoreCreateByteBuf(&packet, size + CoreGetSizeOfVarInt(size));
    // result = CoreWriteVarInt(&packet, size);
    BBStatus status = NetSetupPacket(size, &packet);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't create bytebuf\n");
        return status;
    }
    status = CoreWriteVarInt(&packet, 0x00);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't write varint\n");
        goto FAIL;
    }
    status = CoreWriteVarInt(&packet, PROT_VER);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't write varint\n");
        goto FAIL;
    }
    status = CoreWriteString(&packet, ip, 255);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't write string\n");
        goto FAIL;
    }
    status = CoreWriteU16(&packet, port);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't write u16\n");
        goto FAIL;
    }
    status = CoreWriteVarInt(&packet, 2);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't write varint\n");
        goto FAIL;
    }
    status = NetClientSend(client, packet.data, packet.size);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't send packet\n");
    }
    FAIL:
        CoreDeleteByteBuf(&packet);
        return status;
}




BBStatus NetLoginStart(_IN_ NetClient* client, _IN_ const char* username){
    ByteBuf packet = {0};
    usize nameLength = strlen(username);
    if(nameLength >= 16) nameLength = 16;

    usize size = CoreGetSizeOfVarInt(0x00) + CoreGetSizeOfVarInt(nameLength) + nameLength + sizeof(u64) + sizeof(u64);
    // BBStatus status = CoreCreateByteBuf(&packet, size + CoreGetSizeOfVarInt(size));
    // if(status != BBSTATUS_SUCCESS) DEBUG_FAIL("fail\n");
    // result = CoreWriteVarInt(&packet, size);
    BBStatus status = NetSetupPacket(size, &packet);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't create\n");
        return status;
    }
    status = CoreWriteVarInt(&packet, 0x00);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't write\n");
        goto FAIL;
    }
    status = CoreWriteString(&packet, username, 16);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't write\n");
        goto FAIL;
    }
    status = CoreWriteU64(&packet, 0);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't write\n");
        goto FAIL;
    }
    status = CoreWriteU64(&packet, 0);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't write\n");
        goto FAIL;
    }
    status = NetClientSend(client, packet.data, packet.size);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't send packet\n");
    }
    FAIL:
        CoreDeleteByteBuf(&packet);
        return status;
}


BBStatus NetAcknowledgedPacket(_IN_ NetClient* client){
    ByteBuf packet = {0};
    usize size = CoreGetSizeOfVarInt(0x03);
    BBStatus status = NetSetupPacket(size, &packet);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't create\n");
        return status;
    }
    status = CoreWriteVarInt(&packet, 0x03);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't write\n");
        goto FAIL;
    }
    status = NetClientSend(client, packet.data, packet.size);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't send packet\n");
    }
    client->state = NCSTATE_CONFIG;
FAIL:
    CoreDeleteByteBuf(&packet);
    return status;
}

BBStatus NetKnownPacks(_IN_ NetClient* client, _IN_ char** knownPacks, _IN_ usize amount){
    ByteBuf packet = {0};
    usize size = CoreGetSizeOfVarInt(0x07) + CoreGetSizeOfVarInt(amount);
    for(usize i = 0; i < amount; i++){
        SString str = CoreCreateSString(knownPacks[i]);
        SString strArr[64] = {0};
        usize len = CoreSplitSString(str, ':', 64, strArr);
        for(usize i = 0; i < len; i++){
            usize strLen = strArr[i].length;
            if(strLen > 32767) strLen = 32767;
            size += CoreGetSizeOfVarInt(strLen) + strLen;
        }
    }
    BBStatus status = NetSetupPacket(size, &packet);
    if(status != BBSTATUS_SUCCESS){
        DEBUG_FAIL("couldn't create\n");
        return status;
    }
    status = CoreWriteVarInt(&packet, 0x07);
    if(status != BBSTATUS_SUCCESS){
        DEBUG_FAIL("couldn't write\n");
        goto FAIL;
    }
    status = CoreWriteVarInt(&packet, amount);
    if(status != BBSTATUS_SUCCESS){
        DEBUG_FAIL("couldn't write\n");
        goto FAIL;
    }
    for(usize i = 0; i < amount; i++){
        SString str = CoreCreateSString(knownPacks[i]);
        SString strArr[64] = {0};
        usize len = CoreSplitSString(str, ':', 64, strArr);
        for(usize i = 0; i < len; i++){
            //CoreSStringPuts(strArr[i]);
            status = CoreWriteSString(&packet, strArr[i], 32767);
            if(status != BBSTATUS_SUCCESS){
                DEBUG_FAIL("couldn't write\n");
                goto FAIL;
            }
        }
    }
    status = NetClientSend(client, packet.data, packet.size);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't send packet\n");
    }
    FAIL:
        CoreDeleteByteBuf(&packet);
        return status;
}


BBStatus NetConfigSuccess(_IN_ NetClient* client){
    ByteBuf packet = {0};
    usize size = CoreGetSizeOfVarInt(0x03);
    BBStatus status = NetSetupPacket(size, &packet);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't create\n");
        return status;
    }
    status = CoreWriteVarInt(&packet, 0x03);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't write\n");
        goto FAIL;
    }
    status = NetClientSend(client, packet.data, packet.size);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't send packet\n");
    }
    client->state = NCSTATE_PLAY;
FAIL:
    CoreDeleteByteBuf(&packet);
    return status;
}

BBStatus NetResourcePackDecline(_IN_ NetClient* client, _IN_ u64 firstPartOfUUID, _IN_ u64 secondPartOfUUID){
    ByteBuf packet = {0};
    usize size = CoreGetSizeOfVarInt(0x06) + sizeof(u64) + sizeof(u64) + CoreGetSizeOfVarInt(1);
    BBStatus status = NetSetupPacket(size, &packet);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't create\n");
        return status;
    }
    status = CoreWriteVarInt(&packet, 0x06);
    if(status != BBSTATUS_SUCCESS){
        DEBUG_FAIL("couldn't write\n");
        goto FAIL;
    }
    status = CoreWriteU64(&packet, firstPartOfUUID);
    if(status != BBSTATUS_SUCCESS){
        DEBUG_FAIL("couldn't write\n");
        goto FAIL;
    }
    status = CoreWriteU64(&packet, secondPartOfUUID);
    if(status != BBSTATUS_SUCCESS){
        DEBUG_FAIL("couldn't write\n");
        goto FAIL;
    }
    status = CoreWriteVarInt(&packet, 1);
    if(status != BBSTATUS_SUCCESS){
        DEBUG_FAIL("couldn't write\n");
        goto FAIL;
    }
    status = NetClientSend(client, packet.data, packet.size);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't send packet\n");
    }
    FAIL:
        CoreDeleteByteBuf(&packet);
        return status;
}


BBStatus NetKeepAliveResponse(_IN_ NetClient* client, _IN_ NetClientState state, _IN_ u64 id){
    ByteBuf packet = {0};
    i32 pId = 0;
    if(state == NCSTATE_CONFIG) pId = 4;
    else pId = 27;
    usize size = CoreGetSizeOfVarInt(pId) + sizeof(u64);
    BBStatus status = NetSetupPacket(size, &packet);
    if(status != BBSTATUS_SUCCESS){
        DEBUG_FAIL("couldn't create\n");
        return status;
    }
    status = CoreWriteVarInt(&packet, pId);
    if(status != BBSTATUS_SUCCESS){
        DEBUG_FAIL("couldn't write\n");
        goto FAIL;
    }
    status = CoreWriteU64(&packet, id);
    if(status != BBSTATUS_SUCCESS){
        DEBUG_FAIL("couldn't write\n");
        goto FAIL;
    }
    status = NetClientSend(client, packet.data, packet.size);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't send packet\n");
    }
    DEBUG_INFO("keeping alive\n");
    FAIL:
        CoreDeleteByteBuf(&packet);
        return status;
}

BBStatus NetPong(_IN_ NetClient* client, _IN_ NetClientState state, _IN_ u32 id){
    ByteBuf packet = {0};
    i32 pId = 0;
    if(state == NCSTATE_CONFIG) pId = 5;
    else pId = 44;
    usize size = CoreGetSizeOfVarInt(pId) + sizeof(u32);
    BBStatus status = NetSetupPacket(size, &packet);
    if(status != BBSTATUS_SUCCESS){
        DEBUG_FAIL("couldn't create\n");
        return status;
    }
    status = CoreWriteVarInt(&packet, pId);
    if(status != BBSTATUS_SUCCESS){
        DEBUG_FAIL("couldn't write\n");
        goto FAIL;
    }
    status = CoreWriteU32(&packet, id);
    if(status != BBSTATUS_SUCCESS){
        DEBUG_FAIL("couldn't write\n");
        goto FAIL;
    }
    status = NetClientSend(client, packet.data, packet.size);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("couldn't send packet\n");
    }
    FAIL:
        CoreDeleteByteBuf(&packet);
        return status;
}
