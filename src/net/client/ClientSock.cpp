
#ifdef _WIN32
#include <ws2tcpip.h>
#include <winsock.h>
#include <io.h>
#define close _close
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#include <net/ClientSock.hpp>
#include <brickbox.h>

namespace Net {
    bool ClientSock::connect(const char* ip, uint16_t port){
        #ifdef _WIN32
        WSADATA wsaData = {0};
        int wsaResult = WSAStartup(2, &wsaData);
        if(wsaResult != 0){
            DEBUG_FAIL("wsa initialization failed\n");
            #ifdef _WIN32
            DEBUG_INFO("error: %d\n", WSAGetLastError());
            #endif
            return false;
        }
        #endif
        this->state = ClientState::LOGIN;
        this->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(this->sock == -1){
            DEBUG_FAIL("socket creation failed\n");
            #ifdef _WIN32
            DEBUG_INFO("error: %d\n", WSAGetLastError());
            #endif
            #ifdef _WIN32
            WSACleanup();
            #endif
            return false;
        }
        struct sockaddr_in sa = {.sin_family = AF_INET, .sin_port = htons(port)};
        int result = inet_pton(AF_INET, ip, &sa.sin_addr);
        if(result != 1){
            DEBUG_FAIL("couldn't convert IP address to binary form, %d\n", result);
            close(this->sock);
            this->sock = -1;
            #ifdef _WIN32
            WSACleanup();
            #endif
            return false;
        }
        result = ::connect(this->sock, (struct sockaddr*)&sa, sizeof(sa));
        if(result == -1){
            DEBUG_FAIL("couldn't connect!\n");
            #ifdef _WIN32
            DEBUG_INFO("error: %d\n", WSAGetLastError());
            #endif
            close(this->sock);
            this->sock = -1;
            #ifdef _WIN32
            WSACleanup();
            #endif
            return false;
        }
        return true;
    }

    ClientSock::~ClientSock(){
        if(this->sock != -1){
            close(this->sock);
            #ifdef _WIN32
            WSACleanup();
            #endif
        }
    }

    EventResult ClientSock::checkForEvent(){
        fd_set rfds = {0};
        struct timeval tv = {0};
        FD_ZERO(&rfds);
        FD_SET(this->sock, &rfds);
        int result = select(this->sock + 1, &rfds, NULL, NULL, &tv);
        if(result == -1) return EventResult::FAIL;
        else if(result == 0) return EventResult::NONE;
        else return EventResult::EVENT;
    }

    bool ClientSock::read(uint8_t* buffer, size_t size){
        size_t readFrom = 0;
        uint8_t tempBufForChecking[1] = {0};
        do {
            ssize_t checkResult = recv(this->sock, (char*)tempBufForChecking, 1, MSG_PEEK);
            if(checkResult <= 0){
                DEBUG_INFO("recv failed at checking result\n");
                return false;
            }
            ssize_t s = recv(this->sock, (char*)(buffer + readFrom), size - readFrom, 0);
            if(s == -1){
                DEBUG_INFO("recv failed at getting info\n");
                return false;
            }
            readFrom += s;
        } while(readFrom < size);
        return true;
    }


    bool ClientSock::send(uint8_t* buffer, size_t size){
        size_t sent = 0;
        do {
            ssize_t s = ::send(this->sock, (char*)(buffer + sent), (size - sent), 0);
            if(s == -1){
                return false;
            }
            sent += s;
        } while(sent < size);
        return true;
    }

    uint8_t ClientSock::readByte(){
        uint8_t buffer[1] = {0};
        bool result = this->read(buffer, 1);
        return buffer[0];
    }

    int32_t ClientSock::readVarInt(){
        int32_t value = 0;

        for (int32_t position = 0; position < 32; position += 7) {
            uint8_t currentByte = this->readByte();

            value |= (int32_t)(currentByte & 0x7F) << position;

            if ((currentByte & 0x80) == 0){
                return value;
            }
        }
        DEBUG_INFO("position overflow\n");
        return 0;
    }
}
