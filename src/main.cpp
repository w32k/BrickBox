#include <net/Bytebuf.hpp>
#include <net/ClientSock.hpp>
#include <stdio.h>
#include <chrono>
#include <thread>


int main2(){
    //DEBUG_INFO("Brickbox %s (Implementing %s)\n", VER, MC_VER);
    Net::ClientSock sock;
    std::string ip = "127.0.0.1";
    sock.connect(ip.c_str(), 25565);

    {
        std::string ip = "127.0.0.1";
        size_t size = Net::GetSizeOfVarInt(0x00) + Net::GetSizeOfVarInt(774) +
                    Net::GetSizeOfVarInt(ip.length()) + ip.length() + sizeof(uint16_t) + Net::GetSizeOfVarInt(2);
        Net::Bytebuf buffer(size + Net::GetSizeOfVarInt(size));
        buffer.writeVarInt(size);
        buffer.writeVarInt(0x00);
        buffer.writeVarInt(774);
        buffer.writeString("127.0.0.1", 255);
        buffer.writeU16(25565);
        buffer.writeVarInt(2);
        sock.send((uint8_t*)buffer.getData(), size + Net::GetSizeOfVarInt(size));
    }
    {
        std::string name = "BrickBox";
        size_t size = Net::GetSizeOfVarInt(0x00) + Net::GetSizeOfVarInt(name.length()) + name.length() + sizeof(uint64_t) + sizeof(uint64_t);
        Net::Bytebuf buffer(size + Net::GetSizeOfVarInt(size));
        buffer.writeVarInt(size);
        buffer.writeVarInt(0x00);
        buffer.writeString(name, 16);
        buffer.writeU64(0);
        buffer.writeU64(0);
        sock.send((uint8_t*)buffer.getData(), size + Net::GetSizeOfVarInt(size));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //while(1){continue;}


    return 0;
}
