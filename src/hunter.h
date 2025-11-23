#ifndef HUNTER_H
#define HUNTER_H

#include "game.h"
#include "player.h"
#include <stdbool.h>

struct Hunter {
    WINDOW* body;
    float speed;
    float x;
    float y;
    float dx;
    float dy;
    int width;
    int height;
    int bounces;
    bool alive;
    bool dirCalculated;
};

void bounceHunter(struct Hunter* hunter, struct Game* game);
void initHunter(struct Game* game, struct Hunter* hunter);
void drawHunter(struct Hunter* hunter);
void drawHunters(struct Hunter* hunters, int count);
void calcDirectionToPlayer(struct Hunter* hunter, struct Player* player);
void killHunter(struct Hunter* hunter);
void hunterState(struct Hunter hunters[], struct Player* player, struct Game* game, int hunterCount);
#endif
