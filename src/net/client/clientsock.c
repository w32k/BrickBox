
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

#include <net/client.h>
#include <core/bytebuf.h>










BBStatus NetCreateClient(_IN_ const char* ip, _IN_ uint16_t port, _OUT_ NetClient* client){
    #ifdef _WIN32
    WSADATA wsaData = {0};
    int wsaResult = WSAStartup(2, &wsaData);
    if(wsaResult != 0){
        DEBUG_FAIL("wsa initialization failed\n");
        #ifdef _WIN32
        DEBUG_INFO("error: %d\n", WSAGetLastError());
        #endif
        return BBSTATUS_COULDNT_CREATE_SOCKET;
    }
    #endif
    client->state = NCSTATE_LOGIN;
    client->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(client->sock == -1){
        DEBUG_FAIL("socket creation failed\n");
        #ifdef _WIN32
        DEBUG_INFO("error: %d\n", WSAGetLastError());
        #endif
        return BBSTATUS_COULDNT_CREATE_SOCKET;
    }
    struct sockaddr_in sa = {.sin_family = AF_INET, .sin_port = htons(port)};
    int result = inet_pton(AF_INET, ip, &sa.sin_addr);
    if(result != 1){
        DEBUG_FAIL("couldn't convert IP address to binary form, %d\n", result);
        close(client->sock);
        client->sock = -1;
        return BBSTATUS_IP_CONV_FAILED;
    }
    result = connect(client->sock, (struct sockaddr*)&sa, sizeof(sa));
    if(result == -1){
        DEBUG_FAIL("couldn't connect!\n");
        #ifdef _WIN32
        DEBUG_INFO("error: %d\n", WSAGetLastError());
        #endif
        close(client->sock);
        client->sock = -1;
        return BBSTATUS_CONNECTION_FAILED;
    }
    return BBSTATUS_SUCCESS;
}

void NetCloseClient(_IN_ NetClient* client){
    close(client->sock);
}

BBStatus NetClientCheckForEvent(_IN_ NetClient* client){
    fd_set rfds = {0};
    struct timeval tv = {0};
    FD_ZERO(&rfds);
    FD_SET(client->sock, &rfds);
    int result = select(client->sock + 1, &rfds, NULL, NULL, &tv);
    if(result == -1) return BBSTATUS_COULDNT_CHECK;
    else if(result == 0) return BBSTATUS_NO_EVENT_AVALIABLE;
    else return BBSTATUS_SUCCESS;
}

BBStatus NetClientRead(_IN_ NetClient* client, _IN_ usize size, _OUT_ u8* buffer){
    usize readFrom = 0;
    u8 tempBufForChecking[1] = {0};
    do {
        isize checkResult = recv(client->sock, tempBufForChecking, 1, MSG_PEEK);
        if(checkResult <= 0){
            return BBSTATUS_CONNECTION_ENDED;
        }
        isize s = recv(client->sock, buffer + readFrom, size - readFrom, 0);
        if(s == -1){
            return BBSTATUS_FAILED;
        }
        readFrom += s;
    } while(readFrom < size);
    //DEBUG_INFO("%ld, supposed to read %ld\n", readFrom, size);
    return BBSTATUS_SUCCESS;
}


BBStatus NetClientSend(_IN_ NetClient* client, _IN_ u8* buffer, _IN_ usize size){
    usize sent = 0;
    do {
        //DEBUG_INFO("?\n");
        isize s = send(client->sock, buffer + sent, (size - sent), 0);
        if(s == -1){
            if(errno == EAGAIN || errno == EWOULDBLOCK){
                return BBSTATUS_WOULD_BLOCK;
            } else return BBSTATUS_FAILED;
            return -1;
        }
        sent += s;
    } while(sent < size);
    return BBSTATUS_SUCCESS;
}

BBStatus NetClientReadByte(_IN_ NetClient* client, _OUT_ u8* byte){
    u8 buffer[1] = {0};
    BBStatus stat = NetClientRead(client, 1, buffer);
    *byte = buffer[0];
    return stat;
}

BBStatus NetClientReadVarInt(_IN_ NetClient* client, _OUT_ i32* integer){
    i32 value = 0;

    for (i32 position = 0; position < 32; position += 7) {
        u8 currentByte = 0;
        BBStatus stat = NetClientReadByte(client, &currentByte);
        if(stat != BBSTATUS_SUCCESS){
            return stat;
        }
        value |= (i32)(currentByte & 0x7F) << position;

        if ((currentByte & 0x80) == 0){
            *integer = value;
            return BBSTATUS_SUCCESS;
        }
    }
    return BBSTATUS_POSITION_OVERFLOW;
}
