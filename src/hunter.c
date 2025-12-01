#include "hunter.h"
#include "math.h"
#include "utils.h"
#include <curses.h>

void hunterState(struct Hunter hunters[], struct Player* player, struct Game* game, int hunterCount) {
    for (int i = 0; i < hunterCount; i++) {
        struct Hunter* h = &hunters[i];
        if (!h->alive) continue;
        if (!h->dirCalculated)
            calcDirectionToPlayer(h, player);
        h->x += h->dx * h->speed;
        h->y += h->dy * h->speed;

        bounceHunter(h, game);

        if (!h->alive) continue;
    }
}

void drawHunter(struct Hunter* hunter) {

    char txt[10];
    sprintf(txt, " %d ", hunter->bounces);
    mvwaddstr(hunter->body, 0, 0, "***");
    mvwaddstr(hunter->body, 1, 0, "***");
    mvwaddstr(hunter->body, 2, 0, txt);

    wrefresh(hunter->body);
}

void drawHunters(struct Hunter hunters[], int hunterCount) {
    for (int i = 0; i < hunterCount; i++) {
        struct Hunter* h = &hunters[i];
        if (!h->alive) continue;

        mvwin(h->body, h->y, h->x);
        drawHunter(h);
    }
}

void calcDirectionToPlayer(struct Hunter* hunter, struct Player* player) {
    hunter->dx = (float)player->x - hunter->x;
    hunter->dy = (float)player->y - hunter->y;

    float dist = sqrt(hunter->dx * hunter->dx + hunter->dy * hunter->dy);

    hunter->dx /= dist;
    hunter->dy /= dist;

    hunter->dirCalculated = true;
}

void initHunter(struct Game* game, struct Hunter* hunter) {
    hunter->width = 3;
    hunter->height = 3;
    hunter->x = randInt(0, game->cols - 4);
    hunter->y = 4;
    hunter->speed = 0.57f;
    hunter->bounces = 4;
    hunter->alive = true;
    hunter->body = subwin(game->mainWindow, hunter->height, hunter->width, hunter->y, hunter->x);

    drawHunter(hunter);
}

void killHunter(struct Hunter* hunter) {
    if (hunter->body) {
        delwin(hunter->body);
        hunter->body = NULL;
    }
    hunter->alive = false;
}

void bounceHunter(struct Hunter* hunter, struct Game* game) {
    
    if (hunter->bounces > 0) {

        // left wall
        if (hunter->x <= 1) {
            hunter->x = 1;
            hunter->dx = -hunter->dx;
            hunter->bounces--;
        }

        // right wall
        if (hunter->x >= game->cols - hunter->width - 1) {
            hunter->x = game->cols - hunter->width - 1;
            hunter->dx = -hunter->dx;
            hunter->bounces--;
        }

        // top wall
        if (hunter->y <= 1) {
            hunter->y = 1;
            hunter->dy = -hunter->dy;
            hunter->bounces--;
        }

        // bot wall
        if (hunter->y >= game->rows - game->hudHeight - hunter->height - 1) {
            hunter->y = game->rows - game->hudHeight - hunter->height - 1;
            hunter->dy = -hunter->dy;
            hunter->bounces--;
        }
        
    } else {
        killHunter(hunter);
    }
}