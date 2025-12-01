#include "star.h"
#include "utils.h"
#include "math.h"
#include <curses.h>

void drawStar(struct Star* star) {
    mvwaddstr(star->body, 0, 0, "*");

    wrefresh(star->body);
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
        s->y += 1 * s->speed;
    }
}