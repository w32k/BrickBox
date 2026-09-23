#ifndef SERVERBOUND_H_INCLUDED
#define SERVERBOUND_H_INCLUDED







#include <brickbox.h>






#ifdef __cplusplus
extern "C"
{
#endif
BBStatus NetHandshakePacket(_IN_ NetClient* client, _IN_ const char* ip, _IN_ u16 port);
BBStatus NetLoginStart(_IN_ NetClient* client, _IN_ const char* username);
BBStatus NetAcknowledgedPacket(_IN_ NetClient* client);
BBStatus NetKnownPacks(_IN_ NetClient* client, _IN_ char** knownPacks, _IN_ usize amount);
BBStatus NetConfigSuccess(_IN_ NetClient* client);
BBStatus NetResourcePackDecline(_IN_ NetClient* client, _IN_ u64 firstPartOfUUID, _IN_ u64 secondPartOfUUID);
BBStatus NetKeepAliveResponse(_IN_ NetClient* client, _IN_ NetClientState state, _IN_ u64 id);
#ifdef __cplusplus
}
#endif
#endif
