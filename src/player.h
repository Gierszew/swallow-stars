#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"

struct Player {
    WINDOW* body;
    float speed;
    float x;
    float y;
    int width;
    int height;
    int score;
};

void initPlayer(struct Game* game, struct Player* player);
void drawPlayer(struct Player* player);

#endif
