#include "player.h"
#include <curses.h>

void initPlayer(struct Game* game, struct Player* player) {
    player->width = 5;
    player->height = 3;
    player->score = 0;
    player->hp = 5;
    player->speed = 0.8f;
    player->x = (game->cols - player->width) / 2;
    player->y = (game->rows - player->height) / 2;

    player->movingUp = false;
    player->movingDown = false;
    player->movingLeft = false;
    player->movingRight = false;

    player->body = subwin(game->mainWindow, player->height, player->width, player->y, player->x);

    drawPlayer(player);
}

void drawPlayer(struct Player* player) {
    mvwaddstr(player->body, 0, 0, " /|\\");
    mvwaddstr(player->body, 1, 0, "/ | \\");
    mvwaddstr(player->body, 2, 0, "  |  ");

    wrefresh(player->body);
}

void updatePlayerPosition(struct Game* game, struct Player* player) {
    if (player->movingUp && player->y > 2) 
        player->y -= 1 * player->speed;
    if (player->movingDown && player->y < (game->rows - game->hudHeight - player->height - 1)) 
        player->y += 1 * player->speed;
    if (player->movingLeft && player->x > 2) 
        player->x -= 1 * player->speed;
    if (player->movingRight && player->x < (game->cols - player->width - 1)) 
        player->x += 1 * player->speed;
}