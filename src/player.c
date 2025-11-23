#include "player.h"
#include <curses.h>

void initPlayer(struct Game* game, struct Player* player) {
    player->width = 5;
    player->height = 3;
    player->score = 5;
    player->speed = 1.2f;
    player->x = (game->cols - player->width) / 2;
    player->y = (game->rows - player->height) / 2;

    player->body = subwin(game->mainWindow, player->height, player->width, player->y, player->x);

    drawPlayer(player);
}

void drawPlayer(struct Player* player) {
    mvwaddstr(player->body, 0, 0, " /|\\");
    mvwaddstr(player->body, 1, 0, "/ | \\");
    mvwaddstr(player->body, 2, 0, "  |  ");

    wrefresh(player->body);
}
