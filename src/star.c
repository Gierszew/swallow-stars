#include "star.h"
#include "utils.h"
#include "math.h"
#include <curses.h>

void drawStar(struct Star* star) {
    mvwaddstr(star->body, 0, 0, "*");

    wrefresh(star->body);
}

void drawStars(struct Star* star, int starCount) {
    for (int i = 0; i < starCount; i++) {
        struct Star* s = &star[i];
        if (!s->alive) continue;

        mvwin(s->body, s->y, s->x);
        drawStar(s);
    }
}

void initStar(struct Game* game, struct Star* star) {
    star->width = 1;
    star->height = 1;
    star->x = randInt(0, game->cols - 4);
    star->y = 2;
    star->speed = 0.3f;
    star->alive = true;
    star->body = subwin(game->mainWindow, star->height, star->width, star->y, star->x);
    drawStar(star);
}

void starState(struct Star stars[], struct Player* player, struct Game* game, int starCount) {
    for (int i = 0; i < starCount; i++) {
        struct Star* s = &stars[i];
        if (!s->alive) continue;
        collisionDetect(game, player, s);
        s->y += 1 * s->speed;
    }
}

void collisionDetect(struct Game* game, struct Player* player, struct Star* star) {
    if (star->x <= 1)
        killStar(star);
    if (star->x >= game->cols - star->width - 1)
        killStar(star);
    if (star->y <= 1)
        killStar(star);
    if (star->y >= game->rows - game->hudHeight - star->height - 1)
        killStar(star);
    if (star->alive) {  // Tylko jeśli gwiazda jeszcze żyje
        if (star->x < player->x + player->width &&
            star->x + star->width > player->x &&
            star->y < player->y + player->height &&
            star->y + star->height > player->y) {
            
            // Kolizja wykryta!
            player->score++;  // Dodaj punkty
            killStar(star);  // Usuń gwiazdę
        }
    }
}

void killStar(struct Star* star) {
    if (star->body) {
        delwin(star->body);
        star->body = NULL;
    }
    star->alive = false;
}