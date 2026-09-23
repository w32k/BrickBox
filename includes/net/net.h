#ifndef NET_H_INCLUDED
#define NET_H_INCLUDED









#include <brickbox.h>
#include <net/bytebuf.h>


#ifdef __cplusplus
extern "C"
{
#endif
BBStatus NetSetupPacket(_IN_ bool compressionEnabled, _IN_ usize size, _OUT_ ByteBuf* packet);
BBStatus NetDecompressPacket(_IN_ ByteBuf* iPacket, _IN_ usize pSize, _IN_ usize dSize, _OUT_ ByteBuf* rPacket);
#ifdef __cplusplus
}
#endif


#endif
