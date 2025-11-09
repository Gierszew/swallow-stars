#include <stdlib.h>
#include <stdio.h>
#include <curses.h>

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define SPEED_UP 'o'
#define SPEED_DOWN 'p'

struct Game {
    WINDOW* mainWindow;
    int cols;
    int rows;
};

struct Player {
    WINDOW* body;
    int speedX;
    int speedY;
    int x;
    int y;
    int width;
    int height;
};

void init(struct Game* game) {
    game->mainWindow = initscr();
    if (game->mainWindow == NULL) {
        fprintf(stderr, "Error initialising ncurses.\n");
	    exit(EXIT_FAILURE);
    }
    game->rows = 25;
    game->cols = 80;
    box(game->mainWindow, 0, 0);
    noecho();
    curs_set(0);
}

void drawPlayer(struct Player* player) {
    mvwaddstr(player->body, 0, 0, " /|\\");
    mvwaddstr(player->body, 1, 0, "/ | \\");
    mvwaddstr(player->body, 2, 0, "  |   ");
 
    wrefresh(player->body);
}

void initPlayer(struct Game* game, struct Player* player) {
    player->width = 5;
    player->height = 3;
    player->x = (game->cols - player->width) / 2;
    player->y = (game ->rows - player->height) / 2;
    player->body = subwin(game->mainWindow, player->height, player->width, player->y, player->x);

    drawPlayer(player);    
}



int main() {
    struct Game game;
    struct Player player;
    char ch;
    init(&game);
    initPlayer(&game, &player);
    while ((ch = getch()) != 'q') {
        werase(player.body);
        switch(ch) {
            case UP:
                if (player.y > 1) --player.y;
                break;
            case DOWN:
                if (player.y < (game.rows - player.height - 2)) ++player.y;
                break;
            case LEFT:
                if (player.x > 1) --player.x;
                break;
            case RIGHT:
                if (player.x < (game.cols - player.width - 1)) ++player.x;
                break;
        }
        werase(game.mainWindow);
        box(game.mainWindow, 0, 0);
        wrefresh(game.mainWindow);
        mvwin(player.body, player.y, player.x);
        drawPlayer(&player);
    }

    delwin(player.body);
    delwin(game.mainWindow);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}