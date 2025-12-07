#ifndef GAME_H
#define GAME_H

#include "GameConfig.h"
#include <curses.h>
#include <stdbool.h>
#include <time.h>

struct Player;
struct Taxi;

struct Game {
    WINDOW* mainWindow;
    WINDOW* hudWindow;

    struct GameConfig config;

    FILE* logFile;

    int seed;
    int topScoresCount;
    int cols;
    int rows;
    int hudHeight;
    int level;
    int requiredScore;
    int speed;
    int endTime;
    int requiredScoreToWin;
    int** cellOccupancy;

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
void updateHud(struct Game* game, struct Player* player, int starCount);
void formatTime(time_t seconds, char* buffer, size_t bufferSize);
void endGame(struct Game* game, struct Player* player);
void showWinScreen(struct Game* game, struct Player* player);
void showLoseScreen(struct Game* game, struct Player* player);

bool shouldSpawnHunter(struct Game* game, float seconds);
bool shouldSpawnStar(struct Game* game, float seconds);
bool shouldSpawnTaxi(struct Game* game, float seconds);

#endif
