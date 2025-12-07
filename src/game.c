#include "game.h"
#include "player.h"
#include "taxi.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

static void initColors(void) {
    if (has_colors()){
        start_color();
        init_pair(1,COLOR_BLACK,COLOR_RED);
        init_pair(2,COLOR_BLACK,COLOR_YELLOW);
        init_pair(3,COLOR_RED,-1);
        init_pair(4,COLOR_YELLOW,-1);
        init_pair(5,COLOR_BLACK,COLOR_BLUE);
        init_pair(6, COLOR_GREEN, -1);
    }
}

static void initGameState(struct Game* game) {
    game->running=true;
    game->lastHunterSpawn=time(NULL);
    game->lastStarSpawn=time(NULL);
    game->lastTaxiSpawn=time(NULL);
    game->startTime=time(NULL);

    game->endTime=game->config.endTime;
    game->currentTime=0;

    game->level=1;
    game->requiredScore=game->config.initialScoreQuota;
    game->requiredScoreToWin=game->config.starQuota;
    
    game->mult=1;
    game->huntersToSpawn=game->config.initialHunterCount;
    game->hudHeight=game->config.hudHeight;
    
    game->seed=game->config.seed;
    game->speed=1;
}

void init(struct Game* game) {
    loadConfig(&game->config);
    game->mainWindow=initscr();
    if(!game->mainWindow){
        fprintf(stderr,"Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }
    initGameState(game);
    getmaxyx(game->mainWindow,game->rows,game->cols);
    game->hudWindow=subwin(game->mainWindow,game->hudHeight,game->cols,game->rows-game->hudHeight-1,0);

    for(int i=0;i<game->cols;i++){
        mvwaddch(game->mainWindow,0,i,game->config.borderChar);
        mvwaddch(game->mainWindow,game->rows-1,i,game->config.borderChar);
    }
    for(int i=0;i<game->rows;i++){
        mvwaddch(game->mainWindow,i,0,game->config.borderChar);
        mvwaddch(game->mainWindow,i,game->cols-1,game->config.borderChar);
    }
    nodelay(game->mainWindow,TRUE);
    keypad(game->mainWindow,TRUE);
    noecho();
    curs_set(0);
    use_default_colors();
    initColors();
}

static void drawHudBorders(struct Game* game) {
    for(int i=1;i<game->cols-1;i++){
        mvwaddstr(game->hudWindow,0,i,"#");
        mvwaddstr(game->hudWindow,game->hudHeight - 1,i,"#");
    }
}

void updateHud(struct Game* game, struct Player* player, int starCount) {
    drawHudBorders(game);

    char timeStr[10],timeStr2[10],timeToTaxi[10];
    formatTime(game->currentTime,timeStr,sizeof(timeStr));
    formatTime((time_t)game->endTime,timeStr2,sizeof(timeStr2));

    float taxiTimeLeft=game->config.taxiSpawnInterval-difftime(time(NULL),game->lastTaxiSpawn);
    
    if(taxiTimeLeft<0) taxiTimeLeft=0;
    formatTime((time_t)taxiTimeLeft,timeToTaxi,sizeof(timeToTaxi));

    mvwprintw(game->hudWindow,2,2,"Score: %d/%d",player->score,game->requiredScoreToWin);
    mvwprintw(game->hudWindow,2,15,"Level: %d",game->level);
    mvwprintw(game->hudWindow,2,25,"%s/%s",timeStr,timeStr2);
    mvwprintw(game->hudWindow,2,40,"HP: %d/5",player->hp);
    mvwprintw(game->hudWindow,2,50,"Speed clamp [o/p]: %d/%d",game->speed,game->config.speedBound);
    mvwprintw(game->hudWindow,2,75,"Move by WSAD");
    mvwprintw(game->hudWindow,2,90,"Stars: %d",starCount);
    mvwprintw(game->hudWindow,2,100,"Time for taxi: [e] %s",timeToTaxi);
    
    for(int i=0;i<game->cols;i++) {
        mvwaddch(game->mainWindow,0,i,game->config.borderChar);
        mvwaddch(game->mainWindow,game->rows-1,i,game->config.borderChar);
    }

    for(int i=0;i<game->rows;i++) {
        mvwaddch(game->mainWindow,i,0,game->config.borderChar);
        mvwaddch(game->mainWindow,i,game->cols-1,game->config.borderChar);
    }
    wrefresh(game->mainWindow);
    wrefresh(game->hudWindow);
}

void formatTime(time_t seconds, char* buffer, size_t bufferSize) {
    int minutes = seconds / 60;
    int secs = seconds % 60;
    snprintf(buffer, bufferSize, "%02d:%02d", minutes, secs);
}


void endGame(struct Game* game, struct Player* player) {
    if (game->currentTime >= game->endTime || player->score >= game->requiredScoreToWin) {
        game->running = false;
        showWinScreen(game, player);
    }
}

bool shouldSpawnHunter(struct Game* game, float seconds) {
    time_t now = time(NULL);
    float elapsed = difftime(now, game->lastHunterSpawn);

    if (elapsed >= seconds) {
        game->lastHunterSpawn = now;
        return true;
    }
    return false;
}

bool shouldSpawnStar(struct Game* game, float seconds) {
    time_t now = time(NULL);
    float elapsed = difftime(now, game->lastStarSpawn);
    
    if (elapsed >= seconds) {
        game->lastStarSpawn = now;
        return true;
    }
    return false;
}

bool shouldSpawnTaxi(struct Game* game, float seconds) {
    time_t now = time(NULL);
    float elapsed = difftime(now, game->lastTaxiSpawn);

    if (elapsed >= seconds) {
        game->lastTaxiSpawn = now;
        return true;
    }
    return false;
}

void showLoseScreen(struct Game* game, struct Player* player) {
    werase(game->mainWindow);
    box(game->mainWindow, 0, 0);
    
    int centerX = game->cols / 2;
    int centerY = game->rows / 2;
    
    wattron(game->mainWindow, A_BOLD | COLOR_PAIR(3));
    mvwprintw(game->mainWindow, centerY - 3, centerX - 5, "You lost...");
    wattroff(game->mainWindow, A_BOLD | COLOR_PAIR(3));
    
    mvwprintw(game->mainWindow, centerY, centerX - 10, "Final Score: %d", player->score);
    mvwprintw(game->mainWindow, centerY + 1, centerX - 10, "Level Reached: %d", game->level);
    mvwprintw(game->mainWindow, centerY + 2, centerX - 10, "Time: %02d:%02d", 
              (int)(game->currentTime / 60), (int)(game->currentTime % 60));
    
    mvwprintw(game->mainWindow, centerY + 5, centerX - 15, "Press any key to exit...");
    
    wrefresh(game->mainWindow);
    
    timeout(-1);
    usleep(2000000);
    getch();
    timeout(0);
}

void showWinScreen(struct Game* game, struct Player* player) {
    werase(game->mainWindow);
    box(game->mainWindow, 0, 0);
    
    int centerX = game->cols / 2;
    int centerY = game->rows / 2;
    
    wattron(game->mainWindow, A_BOLD | COLOR_PAIR(6));
    mvwprintw(game->mainWindow, centerY - 3, centerX - 5, "YOU WIN!");
    wattroff(game->mainWindow, A_BOLD | COLOR_PAIR(6));
    
    mvwprintw(game->mainWindow, centerY, centerX - 10, "Final Score: %d", player->score);
    mvwprintw(game->mainWindow, centerY + 1, centerX - 10, "Level Reached: %d", game->level);
    mvwprintw(game->mainWindow, centerY + 2, centerX - 10, "Time: %02d:%02d", 
              (int)(game->currentTime / 60), (int)(game->currentTime % 60));
    
    mvwprintw(game->mainWindow, centerY + 5, centerX - 15, "Press any key to exit...");
    
    wrefresh(game->mainWindow);
    
    timeout(-1);
    usleep(2000000);
    getch();
    timeout(0);
}