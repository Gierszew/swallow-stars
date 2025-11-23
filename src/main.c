#include <math.h>
#include <time.h>
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
#define MAX_HUNTERS 50

struct Game {
    WINDOW* mainWindow;
    FILE* logFile;
    int cols;
    int rows;
    bool running;

    clock_t lastHunterSpawn;
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
    bool alive;
};

int randInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

void init(struct Game* game) {
    game->logFile = fopen("debug.log", "w");
    game->mainWindow = initscr();
    game->running = true;
    game->lastHunterSpawn = clock();
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

bool shouldSpawnHunter(struct Game* game, int seconds) {
    time_t now = time(NULL);
    float elapsed = difftime(now, game->lastHunterSpawn);

    if (elapsed >= seconds) {
        game->lastHunterSpawn = now;
        return true;

    }
    return false;
}


void drawHunter(struct Hunter* hunter) {
    mvwaddstr(hunter->body, 0, 0, "**");
    mvwaddstr(hunter->body, 1, 0, "**");

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
    hunter->width = 2;
    hunter->height = 2;
    hunter->x = randInt(0, game->cols - 4);
    hunter->y = 4;
    hunter->speed = 0.57f;
    hunter->bounces = 4;
    hunter->alive = true;
    hunter->body = subwin(game->mainWindow, hunter->height, hunter->width, hunter->y, hunter->x);

    drawHunter(hunter);
}

void killHunter(struct Hunter* hunter) {
    if (hunter->body) {
        delwin(hunter->body);
        hunter->body = NULL;
    }
    hunter->alive = false;
}

void bounceHunter(struct Hunter* hunter, struct Game* game) {
    
    if (hunter->bounces > 0) {

        // left wall
        if (hunter->x <= 1) {
            hunter->x = 1;
            hunter->dx = -hunter->dx;
            hunter->bounces--;
        }

        // right wall
        if (hunter->x >= game->cols - hunter->width - 1) {
            hunter->x = game->cols - hunter->width - 1;
            hunter->dx = -hunter->dx;
            hunter->bounces--;
        }

        // top wall
        if (hunter->y <= 1) {
            hunter->y = 1;
            hunter->dy = -hunter->dy;
            hunter->bounces--;
        }

        // bot wall
        if (hunter->y >= game->rows - hunter->height - 1) {
            hunter->y = game->rows - hunter->height - 1;
            hunter->dy = -hunter->dy;
            hunter->bounces--;
        }
        
    } else {
        killHunter(hunter);
    }
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

void hunterState(struct Hunter hunters[], struct Player* player, struct Game* game, int hunterCount) {
    for (int i = 0; i < hunterCount; i++) {
        struct Hunter* h = &hunters[i];
        if (!h->alive) continue;
        if (!h->dirCalculated)
            calcDirectionToPlayer(h, player);
        h->x += h->dx * h->speed;
        h->y += h->dy * h->speed;

        bounceHunter(h, game);

        if (!h->alive) continue;
    }
}

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