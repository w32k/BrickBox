#ifndef NET_H_INCLUDED
#define NET_H_INCLUDED









#include <brickbox.h>
#include <core/bytebuf.h>



void NetEnableCompression();
void NetDisableCompression();
bool NetIsCompressionEnabled();
void NetSetCompressionLimit(_IN_ u32 limit);
u32 NetReturnCompressionLimit();
BBStatus NetSetupPacket(_IN_ usize size, _OUT_ ByteBuf* packet);
BBStatus NetDecompressPacket(_IN_ ByteBuf* iPacket, _IN_ usize pSize, _IN_ usize dSize, _OUT_ ByteBuf* rPacket);




#endif
