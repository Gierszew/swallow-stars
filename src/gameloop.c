#include "gameloop.h"
#include "hunter.h"
#include "star.h"
#include "taxi.h"
#include <stdio.h>

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define SPEED_UP 'p'
#define SPEED_DOWN 'o'
#define QUIT 'q'
#define CALL_TAXI 'e'

static void spawnHunters(struct Game* g, struct ArrayData* a) {
    if(!shouldSpawnHunter(g,g->config.hunterSpawnInterval)) return;
    for(int i=0;i<g->huntersToSpawn;i++) {
        if(a->hunterCount>=a->hunterCapacity&&!reallocHunters(a)) {
            fprintf(stderr,"Memory reallocation failed for hunters!\n");
            g->running=false;
            break;
        }
        initHunter(g,&a->hunters[a->hunterCount++]);
    }
}

static void spawnStars(struct Game* g, struct ArrayData* a) {
    if(!shouldSpawnStar(g,g->config.starSpawnInterval)) return;
    if(a->starCount>=a->starCapacity&&!reallocStars(a)) {
        fprintf(stderr,"Memory reallocation failed for stars!\n");
        g->running=false;
        return;
    }
    initStar(g,&a->stars[a->starCount++]);
}

static void spawnTaxi(struct Game* g, struct ArrayData* a) {
    if(a->taxiCount>=a->taxiCapacity&&!reallocTaxis(a)) {
        fprintf(stderr,"Memory reallocation failed for taxis!\n");
        g->running=false;
        return;
    }
    initTaxi(g,&a->taxis[a->taxiCount++]);
}

void handleSpawns(struct Game* game, struct ArrayData* arr) {
    spawnHunters(game,arr);
    spawnStars(game,arr);
}

static void setDirection(struct Player* p,bool up,bool down,bool left,bool right) {
    p->movingUp=up;
    p->movingDown=down;
    p->movingLeft=left;
    p->movingRight=right;
}

void handleInput(char ch, struct Game* g, struct Player* p, struct ArrayData* a) {
    if(ch==ERR)return;
    switch(ch) {
        case UP:setDirection(p,true,false,false,false);break;
        case DOWN:setDirection(p,false,true,false,false);break;
        case LEFT:setDirection(p,false,false,true,false);break;
        case RIGHT:setDirection(p,false,false,false,true);break;
        case SPEED_UP:if(g->speed<g->config.speedBound)g->speed++;break;
        case SPEED_DOWN:if(g->speed>1)g->speed--;break;
        case CALL_TAXI:
            if(shouldSpawnTaxi(g,g->config.taxiSpawnInterval))spawnTaxi(g,a);
            break;
        case QUIT:g->running=false;break;
    }
}

void updateGameState(struct Game* g, struct Player* p, struct ArrayData* a) {
    updatePlayerPosition(g,p);
    hunterState(a->hunters,p,g,a->hunterCount);
    starState(a->stars,p,g,a->starCount);
    taxiState(a->taxis,a->hunters,p,g,a->taxiCount,a->hunterCount);
    if(p->score>=g->requiredScore){
        g->level++;
        g->requiredScore+=5;
    }
}

void renderFrame(struct Game* g, struct Player* p, struct ArrayData* a) {
    eraseEntities(a,p);
    werase(g->mainWindow);
    wrefresh(g->mainWindow);
    mvwin(p->body,p->y,p->x);
    updateHud(g,p,a->starCount);
    drawPlayer(p);
    drawHunters(a->hunters,a->hunterCount);
    drawStars(a->stars,a->starCount);
    drawTaxis(a->taxis,a->taxiCount);
    endGame(g,p);
}
