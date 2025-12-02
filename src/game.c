#include "game.h"
#include "player.h"
#include "taxi.h"
#include <stdio.h>
#include <stdlib.h>

void init(struct Game* game) {
    game->logFile = fopen("debug.log", "w");
    game->mainWindow = initscr();
    game->running = true;
    game->lastHunterSpawn = time(NULL);
    game->lastStarSpawn = time(NULL);
    game->lastTaxiSpawn = time(NULL);
    game->startTime = time(NULL);
    game->currentTime = 0;
    if (!game->mainWindow) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }

    getmaxyx(game->mainWindow, game->rows, game->cols);
    game->level = 1;
    game->requiredScore = 10;
    game->hudHeight = 5;
    game->mult = 1;
    game->huntersToSpawn = 1;

    game->hudWindow = subwin(game->mainWindow, game->hudHeight, game->cols, game->rows - game->hudHeight - 1, 0);

    box(game->mainWindow, 0, 0);
    nodelay(game->mainWindow, TRUE);
    noecho();
    curs_set(0);
}

void updateHud(struct Game* game, struct Player* player) {
    for (int i = 1; i < game->cols - 1; i++) {
        mvwaddstr(game->hudWindow, 0, i, "#");
        mvwaddstr(game->hudWindow, game->hudHeight - 1, i, "#");
    }

    char timeStr[10];
    formatTime(game->currentTime, timeStr, sizeof(timeStr));
    
    mvwprintw(game->hudWindow, 2, 2, "Score: %d/%d", player->score, game->requiredScore);
    mvwprintw(game->hudWindow, 2, 40, "Level: %d", game->level);
    mvwprintw(game->hudWindow, 2, 80, timeStr);
    mvwprintw(game->hudWindow, 2, 110, "HP: %d/5", player->hp);
    wrefresh(game->hudWindow);
}

void formatTime(time_t seconds, char* buffer, size_t bufferSize) {
    int minutes = seconds / 60;
    int secs = seconds % 60;
    snprintf(buffer, bufferSize, "%02d:%02d", minutes, secs);
}

bool shouldSpawnHunter(struct Game* game, float seconds) {
    time_t now = time(NULL);
    float elapsed = difftime(now, game->lastHunterSpawn);

    if (elapsed >= seconds) {
        game->lastHunterSpawn = now;
        return true;
    }
    return false;
}

bool shouldSpawnStar(struct Game* game, float seconds) {
    time_t now = time(NULL);
    float elapsed = difftime(now, game->lastStarSpawn);
    
    if (elapsed >= seconds) {
        game->lastStarSpawn = now;
        return true;
    }
    return false;
}

bool shouldSpawnTaxi(struct Game* game, float seconds) {
    time_t now = time(NULL);
    float elapsed = difftime(now, game->lastTaxiSpawn);

    if (elapsed >= seconds) {
        game->lastTaxiSpawn = now;
        return true;
    }
    return false;
}