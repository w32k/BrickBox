#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED





#include <stdint.h>
#include <stddef.h>


namespace Net {
    enum class ClientState {
        LOGIN,
        CONFIG,
        PLAY,
        STATUS,
        EXIT
    };
    enum class EventResult {
        FAIL,
        NONE,
        EVENT
    };
    class ClientSock {
        private:
            int32_t sock;
            int32_t compressionLimit;
            ClientState state;
            bool compressionEnabled;
        public:
            bool connect(const char* ip, uint16_t port);
            ~ClientSock();

            //BBStatus createClient(const char* ip, uint16_t port, Client* client);
            //void CloseClient(Client* client);
            EventResult checkForEvent();
            bool read(uint8_t* buffer, size_t size);
            bool send(uint8_t* buffer, size_t size);
            uint8_t readByte();
            int32_t readVarInt();
    };
}




#endif
