#ifndef GAME_H
#define GAME_H

#include <curses.h>
#include <stdbool.h>
#include <time.h>

struct Player;
struct Taxi;
struct Game {
    WINDOW* mainWindow;
    WINDOW* hudWindow;
    FILE* logFile;
    int cols;
    int rows;
    int hudHeight;
    int level;
    int requiredScore;
    float mult;
    float huntersToSpawn;
    bool running;
    time_t lastHunterSpawn;
    time_t lastStarSpawn;
    time_t lastTaxiSpawn;
    time_t startTime;
    time_t currentTime;
};

void init(struct Game* game);
void updateHud(struct Game* game, struct Player* player);
void formatTime(time_t seconds, char* buffer, size_t bufferSize);
bool shouldSpawnHunter(struct Game* game, float seconds);
bool shouldSpawnStar(struct Game* game, float seconds);
bool shouldSpawnTaxi(struct Game* game, float seconds);

#endif
