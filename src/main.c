#include "game.h"
#include "player.h"
#include "hunter.h"
#include "utils.h"
#include "star.h"
#include <unistd.h>
#include <stdlib.h>

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define SPEED_UP 'o'
#define SPEED_DOWN 'p'
#define QUIT 'q'
#define MAX_HUNTERS 50


int main() {
    srand(time(NULL));
    struct Game game;
    struct Player player;
    struct Hunter hunters[MAX_HUNTERS];
    for (int i = 0; i < MAX_HUNTERS; ++i) {
        hunters[i].alive = false;
        hunters[i].body = NULL;
        hunters[i].dirCalculated = false;
        hunters[i].dx = hunters[i].dy = 0.0f;
    }
    int hunterCount = 0;
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
            fprintf(game.logFile, "Hello world");
        }
        drawHunters(hunters, hunterCount);
        werase(player.body);
        for (int i = 0; i < hunterCount; i++) {
            werase(hunters[i].body);
        }
        fflush(game.logFile);
        switch(ch) {
            case UP:
                if (player.y > 1) player.y -= 1 * player.speed;
                break;
            case DOWN:
                if (player.y < (game.rows - player.height - 1)) player.y += 1 * player.speed;
                break;
            case LEFT:
                if (player.x > 1) player.x -= 1 * player.speed;
                break;
            case RIGHT:
                if (player.x < (game.cols - player.width - 1)) player.x += 1 * player.speed;
                break;
            case QUIT:
                game.running = false;
                break;
        }
        
        hunterState(hunters, &player, &game, hunterCount);
        
        usleep(16666);
        werase(game.mainWindow);
        box(game.mainWindow, 0, 0);
        wrefresh(game.mainWindow);
        mvwin(player.body, player.y, player.x);
        
        drawPlayer(&player);
    }
    fclose(game.logFile);
    delwin(player.body);
    delwin(game.mainWindow);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}