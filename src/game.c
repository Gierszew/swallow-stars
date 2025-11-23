// game.c
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

void init(struct Game* game) {
    game->logFile = fopen("debug.log", "w");
    game->mainWindow = initscr();
    game->running = true;
    game->lastHunterSpawn = time(NULL);

    if (!game->mainWindow) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }

    getmaxyx(game->mainWindow, game->rows, game->cols);

    box(game->mainWindow, 0, 0);

    nodelay(game->mainWindow, TRUE);
    noecho();
    curs_set(0);
}

bool shouldSpawnHunter(struct Game* game, int seconds) {
    time_t now = time(NULL);
    double elapsed = difftime(now, game->lastHunterSpawn);

    if (elapsed >= seconds) {
        game->lastHunterSpawn = now;
        return true;
    }
    return false;
}
