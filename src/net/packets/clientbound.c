#include <net/packets/clientbound.h>
#include <net/packets/serverbound.h>

#include <net/net.h>






BBStatus NetLoginDisconnect(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    DEBUG_PASS("Exiting during Login Stage!\n");
    client->state = NCSTATE_EXIT;
    return BBSTATUS_SUCCESS;
}

BBStatus NetHello(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    DEBUG_PASS("Server said Hello!!!\n");
    return BBSTATUS_SUCCESS;
}

BBStatus NetLoginFinished(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    BBStatus status = NetAcknowledgedPacket(client);
    return status;
}

BBStatus NetInitCompression(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    NetEnableCompression();
    i32 compLimit = 0;
    BBStatus status = NetReadVarInt(&compLimit, packet);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    NetSetCompressionLimit(compLimit);
    return BBSTATUS_SUCCESS;
}

BBStatus NetCustomQuery(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    // NOT IMPLEMENTED
    return BBSTATUS_NOT_IMPLEMENTED;
}

BBStatus NetCookieRequest(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    // NOT IMPLEMENTED
    return BBSTATUS_NOT_IMPLEMENTED;
}

BBStatus NetCustomPayload(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    // NOT IMPLEMENTED
    return BBSTATUS_NOT_IMPLEMENTED;
}

BBStatus NetConfigDisconnect(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    DEBUG_PASS("Exiting during Config Stage!\n");
    client->state = NCSTATE_EXIT;
    return BBSTATUS_SUCCESS;
}

BBStatus NetFinishConfiguration(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    BBStatus status = NetConfigSuccess(client);
    return status;
}

BBStatus NetKeepAlive(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    u64 result = 0;
    BBStatus status = NetReadU64(&result, packet);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    status = NetKeepAliveResponse(client, client->state, result);
    return status;
}

BBStatus NetPing(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    // NOT IMPLEMENTED
    return BBSTATUS_NOT_IMPLEMENTED;
}

BBStatus NetResetChat(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    // NOT IMPLEMENTED
    return BBSTATUS_NOT_IMPLEMENTED;
}

BBStatus NetRegistryData(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    // NOT IMPLEMENTED
    return BBSTATUS_NOT_IMPLEMENTED;
}

BBStatus NetResourcePackPop(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    // NOT IMPLEMENTED
    return BBSTATUS_NOT_IMPLEMENTED;
}

BBStatus NetResourcePackPush(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    u64 first, second = 0;
    BBStatus status = NetReadU64(&first, packet);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    status = NetReadU64(&second, packet);
    if(status != BBSTATUS_SUCCESS){
        return status;
    }
    DEBUG_INFO("%lu %lu\n", first, second);
    status = NetResourcePackDecline(client, first, second);
    return status;
}

BBStatus NetStoreCookie(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    // NOT IMPLEMENTED
    return BBSTATUS_NOT_IMPLEMENTED;
}

BBStatus NetTransfer(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    // NOT IMPLEMENTED
    return BBSTATUS_NOT_IMPLEMENTED;
}

BBStatus NetUpdateEnabledFeatures(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    char* packs[] = {"minecraft:core:1.21.11"};
    BBStatus status = NetKnownPacks(client, packs, 1);
    return status;
}

BBStatus NetSelectUnknownPacks(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    // NOT IMPLEMENTED
    return BBSTATUS_NOT_IMPLEMENTED;
}

BBStatus NetCustomReportDetails(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    // NOT IMPLEMENTED
    return BBSTATUS_NOT_IMPLEMENTED;
}

BBStatus NetServerLinks(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    // NOT IMPLEMENTED
    return BBSTATUS_NOT_IMPLEMENTED;
}

BBStatus NetClearDialog(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    // NOT IMPLEMENTED
    return BBSTATUS_NOT_IMPLEMENTED;
}

BBStatus NetShowDialog(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    // NOT IMPLEMENTED
    return BBSTATUS_NOT_IMPLEMENTED;
}

BBStatus NetCodeOfConduct(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    // NOT IMPLEMENTED
    return BBSTATUS_NOT_IMPLEMENTED;
}

BBStatus NetUpdateTags(_IN_ NetClient* client, _IN_ ByteBuf* packet){
    // NOT IMPLEMENTED
    return BBSTATUS_NOT_IMPLEMENTED;
}
