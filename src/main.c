#include "game.h"
#include "player.h"
#include "arrays.h"
#include "gameloop.h"
#include <unistd.h>
#include <stdlib.h>


int main() {
    struct Game game;
    struct Player player;
    struct ArrayData arr;
    if(!allocArrays(&arr)){fprintf(stderr,"Memory allocation failed!\n");return EXIT_FAILURE;}
    initArrays(&arr);
    
    init(&game);
    srand(game.seed);
    initPlayer(&game,&player);
    while(game.running){
        char ch=getch();
        if(player.hp<=0)game.running=false;
        getmaxyx(game.mainWindow,game.rows,game.cols);
        game.currentTime=time(NULL)-game.startTime;
        handleSpawns(&game,&arr);
        handleInput(ch,&game,&player,&arr);
        updateGameState(&game,&player,&arr);
        usleep(16666);
        renderFrame(&game,&player,&arr);
    }
    
    fclose(game.logFile);
    cleanupArrays(&arr);
    delwin(player.body);
    delwin(game.mainWindow);
    delwin(game.hudWindow);
    endwin();
    return EXIT_SUCCESS;
}
