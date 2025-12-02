#include "game.h"
#include "player.h"
#include "hunter.h"
#include "star.h"
#include "utils.h"
#include <unistd.h>
#include <stdlib.h>

#define UP          'w'
#define DOWN        's'
#define LEFT        'a'
#define RIGHT       'd'
#define SPEED_UP    'o'
#define SPEED_DOWN  'p'
#define QUIT        'q'
#define MAX_HUNTERS 50
#define MAX_STARS   50


int main() {
    srand(time(NULL));
    struct Game game;
    struct Player player;
    struct Hunter hunters[MAX_HUNTERS];
    struct Star stars[MAX_STARS];
    for (int i = 0; i < MAX_HUNTERS; ++i) {
        hunters[i].alive = false;
        hunters[i].body = NULL;
        hunters[i].dirCalculated = false;
        hunters[i].dx = hunters[i].dy = 0.0f;
    }
    for (int i = 0; i < MAX_STARS; ++i) {
        stars[i].alive = false;
        stars[i].body = NULL;
    }
    int hunterCount = 0;
    int starCount = 0;
    char ch;
    init(&game);
    initPlayer(&game, &player);
    while (game.running) {
        ch = getch();
        if (player.score <= 0)
            game.running = false;
        getmaxyx(game.mainWindow, game.rows, game.cols);
        if (shouldSpawnHunter(&game, 1.0f)) {
            if (hunterCount < MAX_HUNTERS) {
                initHunter(&game, &hunters[hunterCount]);
                hunterCount++;
            }
        }
        if (shouldSpawnStar(&game, 3.0f)) {
            if (starCount < MAX_STARS) {
                initStar(&game, &stars[starCount]);
                starCount++;
            }
            fprintf(game.logFile, "Hello world");   
        }
        drawHunters(hunters, hunterCount);
        drawStars(stars, starCount);
        werase(player.body);
        for (int i = 0; i < hunterCount; i++) {
            werase(hunters[i].body);
        }
        for (int i = 0; i < starCount; i++) {
            werase(stars[i].body);
        }
        fflush(game.logFile);
        if (ch != ERR) {
            switch(ch) {
                case UP:
                    player.movingUp = true;
                    player.movingDown = false;
                    player.movingLeft = false;
                    player.movingRight = false;
                    break;
                case DOWN:
                    player.movingDown = true;
                    player.movingUp = false;
                    player.movingLeft = false;
                    player.movingRight = false;
                    break;
                case LEFT:
                    player.movingDown = false;
                    player.movingUp = false;
                    player.movingLeft = true;
                    player.movingRight = false;
                    break;
                case RIGHT:
                    player.movingDown = false;
                    player.movingUp = false;
                    player.movingRight = true;
                    player.movingLeft = false;
                    break;
                case QUIT:
                    game.running = false;
                    break;
            }
        }
        updatePlayerPosition(&game, &player);
        hunterState(hunters, &player, &game, hunterCount);
        starState(stars, &player, &game, starCount);
        usleep(16666);
        werase(game.mainWindow);
        box(game.mainWindow, 0, 0);
        wrefresh(game.mainWindow);
        mvwin(player.body, player.y, player.x);
        updateHud(&game, &player);
        drawPlayer(&player);
    }
    fclose(game.logFile);
    delwin(player.body);
    delwin(game.mainWindow);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}