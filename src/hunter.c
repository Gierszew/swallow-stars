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
        
        // Sprawdź czy hunter jest ogłuszony
        if (h->stunned) {
            time_t now = time(NULL);
            float stunElapsed = difftime(now, h->stunTimer);
            if (stunElapsed >= 0.5f) {  // 0.5 sekundy zatrzymania
                h->stunned = false;
            }
        }
        
        // Ruch tylko jeśli nie jest ogłuszony
        if (!h->stunned) {
            h->x += h->dx * h->speed;
            h->y += h->dy * h->speed;
        }

        bounceHunter(h, game, player);
        collisionDetectHunter(h, player);
        upgradeHunter(game, h);
        if (!h->alive) continue;
    }
}

void drawHunter(struct Hunter* hunter) {

    char txt[10];
    char ht[4];
    if (hunter->type == 0) {
        wattron(hunter->body, COLOR_PAIR(1));
        sprintf(ht, "%c%c%c", hunter->hunterTemplate, hunter->hunterTemplate, hunter->hunterTemplate);
        sprintf(txt, " %d ", hunter->bounces);
        mvwaddstr(hunter->body, 0, 0, ht);
        mvwaddstr(hunter->body, 1, 0, ht);
        mvwaddstr(hunter->body, 2, 0, txt);
        wattroff(hunter->body, COLOR_PAIR(1));
    } else if (hunter->type == 1) {
        wattron(hunter->body, COLOR_PAIR(5));
        sprintf(ht, "%c", hunter->hunterTemplate);
        sprintf(txt, "%d", hunter->bounces);
        mvwaddstr(hunter->body, 0, 0, ht);
        mvwaddstr(hunter->body, 1, 0, ht);
        mvwaddstr(hunter->body, 2, 0, txt);
        wattroff(hunter->body, COLOR_PAIR(5));
    }

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
    hunter->type = randInt(0, 1);

    if (hunter->type == 0) {
        hunter->width = 3;
        hunter->height = 3;
        hunter->damage = game->config.hunterDamage;
    } else if (hunter->type == 1) {
        hunter->width = 1;
        hunter->height = 3;
        hunter->damage = game->config.hunterDamage + 1;
    }
    hunter->hunterTemplate = game->config.hunterTemplate;
    hunter->x = randInt(0, game->cols - 4);
    hunter->y = hunter->height + 1;
    hunter->speed = 0.57f;
    hunter->bounces = 4 * game->mult;
    hunter->alive = true;
    hunter->stunned = false;
    hunter->body = subwin(game->mainWindow, hunter->height, hunter->width, hunter->y, hunter->x);

    hunter->hunterTimerToUpgrade = time(NULL);
    hunter->requiredSecondsToUpgrade = 10;
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
        // Nie sprawdzaj odbić jeśli hunter jest już ogłuszony
        if (hunter->stunned) {
            return;
        }
        
        bool bounced = false;

        // left wall
        if (hunter->x <= 1) {
            hunter->x = 1;
            hunter->dx = -hunter->dx;
            bounced = true;
        }

        // right wall
        if (hunter->x >= game->cols - hunter->width - 1) {
            hunter->x = game->cols - hunter->width - 1;
            hunter->dx = -hunter->dx;
            bounced = true;
        }

        // top wall
        if (hunter->y <= 1) {
            hunter->y = 1;
            hunter->dy = -hunter->dy;
            bounced = true;
        }

        // bot wall
        if (hunter->y >= game->rows - game->hudHeight - hunter->height - 1) {
            hunter->y = game->rows - game->hudHeight - hunter->height - 2;
            hunter->dy = -hunter->dy;
            bounced = true;
        }
        
        // Jeśli nastąpiło odbicie, odejmij bounce i zastosuj stun
        if (bounced) {
            hunter->bounces--;
            hunter->stunned = true;
            hunter->stunTimer = time(NULL);
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
            player->hp -= hunter->damage;
            killHunter(hunter);
        }
    }
}

void upgradeHunter(struct Game* game, struct Hunter* hunter) {
    time_t now = time(NULL);
    float elapsed = difftime(now, hunter->hunterTimerToUpgrade);

    if (elapsed >= hunter->requiredSecondsToUpgrade) {
        game->mult += 0.3f;
        hunter->hunterTimerToUpgrade = now;
        game->huntersToSpawn += 0.5f;
    }
}