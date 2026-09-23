#ifndef CLIENTBOUND_H_INCLUDED
#define CLIENTBOUND_H_INCLUDED







#include <brickbox.h>
#include <net/client.h>
#include <core/bytebuf.h>

#ifdef __cplusplus
extern "C"
{
#endif
BBStatus NetLoginDisconnect(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetHello(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetLoginFinished(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetInitCompression(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetCustomQuery(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetCookieRequest(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetCustomPayload(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetConfigDisconnect(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetFinishConfiguration(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetKeepAlive(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetPing(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetResetChat(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetRegistryData(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetResourcePackPop(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetResourcePackPush(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetStoreCookie(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetTransfer(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetUpdateEnabledFeatures(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetSelectUnknownPacks(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetCustomReportDetails(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetServerLinks(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetClearDialog(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetShowDialog(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetCodeOfConduct(_IN_ NetClient* client, _IN_ ByteBuf* packet);
BBStatus NetUpdateTags(_IN_ NetClient* client, _IN_ ByteBuf* packet);
#ifdef __cplusplus
}
#endif






#endif
