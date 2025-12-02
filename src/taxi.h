#ifndef TAXI_H
#define TAXI_H

#include "game.h"
#include "player.h"

struct Taxi {
    WINDOW* body;
    float x;
    float y;
    float speed;
    int width;
    int height;
    bool alive;
};

void taxiState(struct Taxi taxis[], struct Player* player, struct Game* game, int taxiCount);
void initTaxi(struct Game* game, struct Taxi* taxi);
void drawTaxi(struct Taxi* taxi);
void drawTaxis(struct Taxi taxis[], int taxiCount);
void killTaxi(struct Taxi* taxi);
void collisionDetectTaxi(struct Game* game, struct Player* player, struct Taxi* taxi);
#endif