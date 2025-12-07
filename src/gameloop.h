#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "game.h"
#include "player.h"
#include "arrays.h"

void handleSpawns(struct Game* game, struct ArrayData* arr);
void handleInput(char ch, struct Game* game, struct Player* player, struct ArrayData* arr);
void updateGameState(struct Game* game, struct Player* player, struct ArrayData* arr);
void renderFrame(struct Game* game, struct Player* player, struct ArrayData* arr);

#endif
