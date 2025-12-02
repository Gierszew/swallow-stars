#ifndef GAME_H
#define GAME_H

#include <curses.h>
#include <stdbool.h>
#include <time.h>

struct Player;

struct Game {
    WINDOW* mainWindow;
    WINDOW* hudWindow;
    FILE* logFile;
    int cols;
    int rows;
    int hudHeight;
    int level;
    bool running;
    time_t lastHunterSpawn;
    time_t lastStarSpawn;
};

void init(struct Game* game);
void updateHud(struct Game* game, struct Player* player);
bool shouldSpawnHunter(struct Game* game, float seconds);
bool shouldSpawnStar(struct Game* game, float seconds);

#endif
