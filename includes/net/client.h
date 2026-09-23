#ifndef NETCLIENT_H_INCLUDED
#define NETCLIENT_H_INCLUDED




#include <brickbox.h>
#include <net/bytebuf.h>

typedef enum _NetClientState {
    NCSTATE_LOGIN,
    NCSTATE_CONFIG,
    NCSTATE_PLAY,
    NCSTATE_EXIT
} NetClientState;

typedef struct _NetClient {
    int sock;
    NetClientState state;
} NetClient;

typedef BBStatus (*PacketHandlerCallback)(_IN_ NetClient* client, _IN_ ByteBuf* packet);

#ifdef __cplusplus
extern "C"
{
#endif
BBStatus NetCreateClient(_IN_ const char* ip, _IN_ uint16_t port, _OUT_ NetClient* client);
void NetCloseClient(_IN_ NetClient* client);
BBStatus NetClientCheckForEvent(_IN_ NetClient* client);
BBStatus NetClientRead(_IN_ NetClient* client, _IN_ usize size, _OUT_ u8* buffer);
BBStatus NetClientSend(_IN_ NetClient* client, _IN_ u8* buffer, _IN_ usize size);
BBStatus NetClientReadByte(_IN_ NetClient* client, _OUT_ u8* byte);
BBStatus NetClientReadVarInt(_IN_ NetClient* client, _OUT_ i32* integer);
BBStatus NetClientHandleEvents(_IN_ NetClient* client);
#ifdef __cplusplus
}
#endif





#endif
