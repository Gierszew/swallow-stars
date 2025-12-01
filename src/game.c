// game.c
#include "game.h"
#include "player.h"
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

    game->hudHeight = 3;

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
    mvwprintw(game->hudWindow, 1, 2, "Score: %d", player->score);
    wrefresh(game->hudWindow);
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
