#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define SPEED_UP 'o'
#define SPEED_DOWN 'p'
#define QUIT 'q'

struct Game {
    WINDOW* mainWindow;
    FILE* logFile;
    int cols;
    int rows;
    bool running;
};

struct Player {
    WINDOW* body;
    float speed;
    float x;
    float y;
    int moveX;
    int moveY;
    int width;
    int height;
    int score;
    int hp;
};

struct Hunter {
    WINDOW* body;
    float speed;
    int direction;
    float x;
    float y;
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
    game->running = true;
    if (game->mainWindow == NULL) {
        fprintf(stderr, "Error initialising ncurses.\n");
	    exit(EXIT_FAILURE);
    }
    getmaxyx(game->mainWindow, game->rows, game->cols);
    box(game->mainWindow, 0, 0);

    nodelay(game->mainWindow, TRUE);
    noecho();
    curs_set(0);
}

void drawHunter(struct Hunter* hunter) {
    mvwaddstr(hunter->body, 0, 0, "**");
    mvwaddstr(hunter->body, 1, 0, "**");

    wrefresh(hunter->body);
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
    hunter->width = 2;
    hunter->height = 2;
    hunter->x = 4;
    hunter->y = 4;
    hunter->speed = 0.57f;
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
    player->speed = 1.2;
    player->x = (game->cols - player->width) / 2;
    player->y = (game ->rows - player->height) / 2;
    player->body = subwin(game->mainWindow, player->height, player->width, player->y, player->x);

    drawPlayer(player);    
}

int main() {
    struct Game game;
    struct Player player;
    struct Hunter hunter;
    struct Hunter* hunters;
    char ch;
    init(&game);
    initPlayer(&game, &player);
    initHunter(&game, &hunter);
    while (game.running) {
        ch = getch();
        if (player.score <= 0)
            game.running = false;
        getmaxyx(game.mainWindow, game.rows, game.cols);
        if (!hunter.dirCalculated)
            calcDirectionToPlayer(&hunter, &player);
        werase(player.body);
        werase(hunter.body);
        fprintf(game.logFile, "Hunter pos: dx=%f dy=%f x=%f y=%f\n", hunter.dx, hunter.dy, hunter.x, hunter.y);
        fprintf(game.logFile, "Player pos: x=%f y=%f\n", player.x, player.y);
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
        hunter.x += hunter.dx * hunter.speed;
        hunter.y += hunter.dy * hunter.speed;

        if (hunter.x < 1) hunter.x = 1;
        if (hunter.y < 1) hunter.y = 1;
        if (hunter.x > game.cols - hunter.width - 1) hunter.x = game.cols - hunter.width - 1;
        if (hunter.y > game.rows - hunter.height - 1) hunter.y = game.rows - hunter.height - 1;
        
        usleep(16666);
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