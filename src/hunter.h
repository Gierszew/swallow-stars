#ifndef HUNTER_H
#define HUNTER_H

#include "game.h"
#include "player.h"
#include <stdbool.h>
#include <time.h>

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
    time_t hunterTimer;
    int requiredSecondsToUpgrade;
};

void bounceHunter(struct Hunter* hunter, struct Game* game, struct Player* player);
void initHunter(struct Game* game, struct Hunter* hunter);
void drawHunter(struct Hunter* hunter);
void drawHunters(struct Hunter hunters[], int count);
void calcDirectionToPlayer(struct Hunter* hunter, struct Player* player);
void killHunter(struct Hunter* hunter);
void hunterState(struct Hunter hunters[], struct Player* player, struct Game* game, int hunterCount);
void collisionDetectHunter(struct Hunter* hunter, struct Player* player);
void upgradeHunter(struct Game* game, struct Hunter* hunter);
#endif