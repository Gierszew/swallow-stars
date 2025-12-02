#include "game.h"
#include "player.h"
#include "hunter.h"
#include "star.h"
#include "taxi.h"
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
#define MAX_HUNTERS 5000
#define MAX_STARS   5000
#define MAX_TAXIS   5000


int main() {
    srand(time(NULL));
    struct Game game;
    struct Player player;
    struct Hunter hunters[MAX_HUNTERS];
    struct Star stars[MAX_STARS];
    struct Taxi taxis[MAX_TAXIS];
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
    for (int i = 0; i < MAX_TAXIS; ++i) {
        taxis[i].alive = false;
        taxis[i].body = NULL;
    }
    int hunterCount = 0;
    int starCount = 0;
    int taxiCount = 0;
    char ch;
    init(&game);
    initPlayer(&game, &player);
    while (game.running) {
        ch = getch();
        if (player.hp <= 0)
            game.running = false;
        getmaxyx(game.mainWindow, game.rows, game.cols);

        game.currentTime = time(NULL) - game.startTime;
        
        if (shouldSpawnHunter(&game, 8.0f)) {
            if (hunterCount < MAX_HUNTERS) {
                for (int i = 0; i < game.huntersToSpawn; i++) {
                    initHunter(&game, &hunters[hunterCount]);
                    hunterCount++;
                }
            }
        }
        if (shouldSpawnStar(&game, 1.0f)) {
            if (starCount < MAX_STARS) {
                initStar(&game, &stars[starCount]);
                starCount++;
            }
        }
        if (shouldSpawnTaxi(&game, 25.0f)) {
            if (taxiCount < MAX_TAXIS) {
                initTaxi(&game, &taxis[taxiCount]);
                taxiCount++;
            }
            fprintf(game.logFile, "Hello world");   
        }
        drawHunters(hunters, hunterCount);
        drawStars(stars, starCount);
        drawTaxis(taxis, taxiCount);
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
        taxiState(taxis, &player, &game, taxiCount);

        if (player.score >= game.requiredScore) {
            game.level++;
            game.requiredScore = 5 * game.level;
            player.score = 0;
        }
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