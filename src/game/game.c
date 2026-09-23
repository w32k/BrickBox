#include <game/game.h>



// NOT THREAD SAFE, TODO: MAKE IT THREAD SAFE!!!!!!!!!!

static bool isGameRunning = TRUE;





void GameEnd(){
    isGameRunning = FALSE;
}

bool GameIsRunning(){
    return isGameRunning;
}
