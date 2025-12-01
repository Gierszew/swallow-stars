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
    bool movingUp, movingDown, movingLeft, movingRight;
};

void initPlayer(struct Game* game, struct Player* player);
void drawPlayer(struct Player* player);
void updatePlayerPosition(struct Game* game, struct Player* player);

#endif
