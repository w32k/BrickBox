#include <brickbox.h>
#include <core/bytebuf.h>
#include <net/client.h>
#include <net/net.h>
#include <game/game.h>
#include <net/packets/serverbound.h>
#include <unistd.h>


i32 main(){
    DEBUG_INFO("Brickbox %s (Implementing %s)\n", VER, MC_VER);

    NetClient client = {0};
    BBStatus status = NetCreateClient("127.0.0.1", 25565, &client);
    if(status != BBSTATUS_SUCCESS){
        DEBUG_FAIL("couldn't connect! %d\n", status);
        return 1;
    }
    DEBUG_PASS("connected to server\n");
    status = NetHandshakePacket(&client, "67.67.67.67", 67);
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("??\n");
        return 2;
    }
    status = NetLoginStart(&client, "brickbox");
    if(status != BBSTATUS_SUCCESS) {
        DEBUG_FAIL("??\n");
        return 3;
    }

    while(GameIsRunning()){
        if(NetClientCheckForEvent(&client) == BBSTATUS_SUCCESS){
            BBStatus status = NetClientHandleEvents(&client);
            if(status != BBSTATUS_SUCCESS){
                switch(status){
                    case BBSTATUS_WOULD_BLOCK:
                    case BBSTATUS_NOT_IMPLEMENTED:
                    case BBSTATUS_NO_EVENT_AVALIABLE:{
                        break;
                    }
                    default:{
                        DEBUG_FAIL("error while handling network events: %d\n", status);
                        GameEnd();
                        break;
                    }
                }
            }
        }
    }
    NetCloseClient(&client);
    return 0;
}
