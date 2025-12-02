#include "taxi.h"
#include "utils.h"
#include <curses.h>

void taxiState(struct Taxi taxis[], struct Player* player, struct Game* game, int taxiCount) {
    for (int i = 0; i < taxiCount; i++) {
        struct Taxi* t = &taxis[i];
        if (!t->alive) continue;
        collisionDetectTaxi(game, player, t);
        t->y += 1 * t->speed;
    }
}

void initTaxi(struct Game* game, struct Taxi* taxi) {
    taxi->width = 1;
    taxi->height = 2;
    taxi->x = randInt(0, game->cols - 4);
    taxi->y = 3;
    taxi->speed = 0.2f;
    taxi->alive = true;
    taxi->body = subwin(game->mainWindow, taxi->height, taxi->width, taxi->y, taxi->x);
    drawTaxi(taxi);
}

void drawTaxi(struct Taxi* taxi) {
    mvwaddstr(taxi->body, 0, 0, "*");
    mvwaddstr(taxi->body, 1, 0, "*");

    wrefresh(taxi->body);
}

void drawTaxis(struct Taxi taxis[], int taxiCount) {
    for (int i = 0; i < taxiCount; i++) {
        struct Taxi* t = &taxis[i];
        if (!t->alive) continue;

        mvwin(t->body, t->y, t->x);
        drawTaxi(t);
    }
}

void collisionDetectTaxi(struct Game* game, struct Player* player, struct Taxi* taxi) {
    if (taxi->x <= 1)
        killTaxi(taxi);
    if (taxi->x >= game->cols - taxi->width - 1)
        killTaxi(taxi);
    if (taxi->y <= 1)
        killTaxi(taxi);
    if (taxi->y >= game->rows - game->hudHeight - taxi->height - 1)
        killTaxi(taxi);
    if (taxi->alive) {
        if (taxi->x < player->x + player->width &&
            taxi->x + taxi->width > player->x &&
            taxi->y < player->y + player->height &&
            taxi->y + taxi->height > player->y) {
            
            game->mult = 1;
            game->huntersToSpawn = 1;

            killTaxi(taxi);
        }
    }
}

void killTaxi(struct Taxi* taxi) {
    if (taxi->body) {
        delwin(taxi->body);
        taxi->body = NULL;
    }
    taxi->alive = false;
}