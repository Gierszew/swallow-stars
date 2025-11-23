#ifndef STAR_H
#define STAR_H

#include "game.h"

struct Star {
    WINDOW* body;
    float x;
    float y;
    float speed;
    int width;
    int height;
    bool alive;
};

void initStar(struct Game* game, struct Star* star);
void drawStar(struct Star* star);
void starState(struct Star stars[], struct Player* player, struct Game* game, int starCount);
void killStar(struct Star* star);

#endif