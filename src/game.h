// game.h
#ifndef GAME_H
#define GAME_H

#include <curses.h>
#include <stdbool.h>
#include <time.h>

struct Game {
    WINDOW* mainWindow;
    FILE* logFile;
    int cols;
    int rows;
    bool running;
    time_t lastHunterSpawn;
};

void init(struct Game* game);
bool shouldSpawnHunter(struct Game* game, int seconds);

#endif
