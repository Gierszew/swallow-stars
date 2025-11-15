#include <math.h>
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
    FILE* logFile;
    int cols;
    int rows;
};

struct Player {
    WINDOW* body;
    int speed;
    int x;
    int y;
    int width;
    int height;
    int score;
    int hp;
};

struct Hunter {
    WINDOW* body;
    int speed;
    int direction;
    float fx;
    float fy;
    int x;
    int y;
    float dx;    // direction to player x
    float dy;    // direction to player y
    int width;
    int height;
    int bounces;
    bool dirCalculated;
};

void init(struct Game* game) {
    game->logFile = fopen("debug.log", "w");
    game->mainWindow = initscr();
    if (game->mainWindow == NULL) {
        fprintf(stderr, "Error initialising ncurses.\n");
	    exit(EXIT_FAILURE);
    }
    getmaxyx(game->mainWindow, game->rows, game->cols);
    box(game->mainWindow, 0, 0);
    noecho();
    curs_set(0);
}

void drawHunter(struct Hunter* hunter) {
    mvwaddstr(hunter->body, 0, 0, "**");
    mvwaddstr(hunter->body, 1, 0, "**");

    wrefresh(hunter->body);
}

void calcDirectionToPlayer(struct Hunter* hunter, struct Player* player) {
    hunter->dx = (float)player->x - hunter->fx;
    hunter->dy = (float)player->y - hunter->fy;

    float dist = sqrt(hunter->dx * hunter->dx + hunter->dy * hunter->dy);

    hunter->dx /= dist;
    hunter->dy /= dist;

    hunter->fx = (hunter->dx * hunter->speed) * 5;
    hunter->fy = (hunter->dy * hunter->speed) * 10;
    hunter->dirCalculated = true;
}

void initHunter(struct Game* game, struct Hunter* hunter) {
    hunter->width = 2;
    hunter->height = 2;
    hunter->x = 4;
    hunter->y = 4;
    hunter->fx = (float)hunter->x;
    hunter->fy = (float)hunter->y;
    hunter->speed = 1.0f;
    hunter->body = subwin(game->mainWindow, hunter->height, hunter->width, hunter->y, hunter->x);

    drawHunter(hunter);
}

void killHunter(struct Hunter* hunter) {
    delwin(hunter->body);
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
    player->score = 5;
    player->x = (game->cols - player->width) / 2;
    player->y = (game ->rows - player->height) / 2;
    player->body = subwin(game->mainWindow, player->height, player->width, player->y, player->x);

    drawPlayer(player);    
}

int main() {
    struct Game game;
    struct Player player;
    struct Hunter hunter;
    char ch;
    init(&game);
    initPlayer(&game, &player);
    initHunter(&game, &hunter);
    while ((ch = getch()) != 'q' && player.score > 0) {

        getmaxyx(game.mainWindow, game.rows, game.cols);
        if (!hunter.dirCalculated)
            calcDirectionToPlayer(&hunter, &player);
        werase(player.body);
        werase(hunter.body);
        fprintf(game.logFile, "Hunter pos: dx=%f dy=%f fx=%f fy=%f x=%d y=%d\n", hunter.dx, hunter.dy, hunter.fx, hunter.fy, hunter.x, hunter.y);
        fflush(game.logFile);
        switch(ch) {
            case UP:
                if (player.y > 1) --player.y;
                break;
            case DOWN:
                if (player.y < (game.rows - player.height - 1)) ++player.y;
                break;
            case LEFT:
                if (player.x > 1) --player.x;
                break;
            case RIGHT:
                if (player.x < (game.cols - player.width - 1)) ++player.x;
                break;
        }
        /* hunter.fx += hunter.dx * hunter.speed;
        hunter.fy += hunter.dy * hunter.speed; */
        hunter.x += (int)hunter.fx;
        hunter.y += (int)hunter.fy;

        if (hunter.x < 1) hunter.x = 1;
        if (hunter.y < 1) hunter.y = 1;
        if (hunter.x > game.cols - hunter.width - 1) hunter.x = game.cols - hunter.width - 1;
        if (hunter.y > game.rows - hunter.height - 1) hunter.y = game.rows - hunter.height - 1;
        
        werase(game.mainWindow);
        box(game.mainWindow, 0, 0);
        wrefresh(game.mainWindow);
        mvwin(player.body, player.y, player.x);
        mvwin(hunter.body, hunter.y, hunter.x);
        drawHunter(&hunter);
        drawPlayer(&player);
    }
    fclose(game.logFile);
    delwin(player.body);
    delwin(game.mainWindow);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}