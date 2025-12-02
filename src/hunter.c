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

        bounceHunter(h, game, player);
        collisionDetectHunter(h, player);
        upgradeHunter(game, h);
        if (!h->alive) continue;
    }
}

void drawHunter(struct Hunter* hunter) {

    char txt[10];
    sprintf(txt, " %d ", hunter->bounces);

    wattron(hunter->body, COLOR_PAIR(1));

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
    hunter->bounces = 4 * game->mult;
    hunter->alive = true;
    hunter->body = subwin(game->mainWindow, hunter->height, hunter->width, hunter->y, hunter->x);

    hunter->hunterTimer = time(NULL);
    hunter->requiredSecondsToUpgrade = 10;

    if (has_colors()) {
        start_color();

        init_pair(1, COLOR_BLACK, COLOR_RED);
    }

    drawHunter(hunter);
}

void killHunter(struct Hunter* hunter) {
    if (hunter->body) {
        delwin(hunter->body);
        hunter->body = NULL;
    }
    hunter->alive = false;
}

void bounceHunter(struct Hunter* hunter, struct Game* game, struct Player* player) {
    
    if (hunter->bounces > 0) {

        // left wall
        if (hunter->x <= 1) {
            hunter->x = 1;
            hunter->dx = -hunter->dx;
            hunter->bounces--;
            calcDirectionToPlayer(hunter, player);
        }

        // right wall
        if (hunter->x >= game->cols - hunter->width - 1) {
            hunter->x = game->cols - hunter->width - 1;
            hunter->dx = -hunter->dx;
            hunter->bounces--;
            calcDirectionToPlayer(hunter, player);
        }

        // top wall
        if (hunter->y <= 1) {
            hunter->y = 1;
            hunter->dy = -hunter->dy;
            hunter->bounces--;
            calcDirectionToPlayer(hunter, player);
        }

        // bot wall
        if (hunter->y >= game->rows - game->hudHeight - hunter->height - 1) {
            hunter->y = game->rows - game->hudHeight - hunter->height - 1;
            hunter->dy = -hunter->dy;
            hunter->bounces--;
            calcDirectionToPlayer(hunter, player);
        }
        
    } else {
        killHunter(hunter);
    }
}

void collisionDetectHunter(struct Hunter* hunter, struct Player* player) {
    if (hunter->alive) {
        if (hunter->x < player->x + player->width &&
            hunter->x + hunter->width > player->x &&
            hunter->y < player->y + player->height &&
            hunter->y + hunter->height > player->y) {
            killHunter(hunter);
            player->hp--;
        }
    }
}

void upgradeHunter(struct Game* game, struct Hunter* hunter) {
    time_t now = time(NULL);
    float elapsed = difftime(now, hunter->hunterTimer);

    if (elapsed >= hunter->requiredSecondsToUpgrade) {
        game->mult += 0.3f;
        hunter->hunterTimer = now;
        game->huntersToSpawn += 0.5f;
    }
}