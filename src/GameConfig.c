#include "GameConfig.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void setDefaults(struct GameConfig* config) {
    config->hudHeight = 5;
    
    config->initialScoreQuota = 10;
    config->scoreQuotaMultiplier = 5;
    config->playerMaxHP = 5;
    
    config->hunterSpawnInterval = 8.0f;
    config->starSpawnInterval = 1.0f;
    config->taxiSpawnInterval = 25.0f;
    
    config->initialHunterCount = 1;
    config->hunterSpeed = 0.57f;
    config->hunterInitialBounces = 4;
    
    config->seed = (unsigned int)time(NULL);

    config->borderChar = '#';
}

static void parseQuotas(struct GameConfig* c,char* k,char* v){
    if(!strcmp(k,"initialScoreQuota"))c->initialScoreQuota=atoi(v);
    else if(!strcmp(k,"scoreQuotaMultiplier"))c->scoreQuotaMultiplier=atoi(v);
    else if(!strcmp(k,"playerMaxHP"))c->playerMaxHP=atoi(v);
    else if(!strcmp(k,"starQuota"))c->starQuota=atoi(v);
}

static void parseTimers(struct GameConfig* c,char* k,char* v){
    if(!strcmp(k,"hunterSpawnInterval"))c->hunterSpawnInterval=atof(v);
    else if(!strcmp(k,"starSpawnInterval"))c->starSpawnInterval=atof(v);
    else if(!strcmp(k,"taxiSpawnInterval"))c->taxiSpawnInterval=atof(v);
    else if(!strcmp(k,"endTime"))c->endTime=atoi(v);
}

static void parseHunters(struct GameConfig* c,char* k,char* v){
    if(!strcmp(k,"initialHunterCount"))c->initialHunterCount=atoi(v);
    else if(!strcmp(k,"hunterSpeed"))c->hunterSpeed=atof(v);
    else if(!strcmp(k,"hunterInitialBounces"))c->hunterInitialBounces=atoi(v);
    else if(!strcmp(k,"hunterDamage"))c->hunterDamage=atoi(v);
    else if(!strcmp(k,"hunterTemplate"))c->hunterTemplate=v[0];
}

static void parseSeed(struct GameConfig* c,char* v){
    if(!strcmp(v,"random"))c->seed=(unsigned int)time(NULL);
    else c->seed=(unsigned int)atoi(v);
}

static void parseOthers(struct GameConfig* c,char* k,char* v){
    if(!strcmp(k,"hudHeight"))c->hudHeight=atoi(v);
    else if(!strcmp(k,"speedBound"))c->speedBound=atoi(v);
    else if (!strcmp(k, "starValue")) c->starValue = atoi(v);
}

static void parseMap(struct GameConfig* c, char* k, char* v) {
    if (!strcmp(k, "borderChar")) c->borderChar = v[0]; 
}

static void parseKeyValue(struct GameConfig* c,char* k,char* v){
    if(!strcmp(k,"seed")) parseSeed(c,v);
    else {
        parseOthers(c, k, v);
        parseQuotas(c, k, v);
        parseTimers(c, k, v);
        parseHunters(c, k, v);
        parseMap(c, k, v);
    }
}

void loadConfig(struct GameConfig* config) {
    config->filename = "config.txt";
    setDefaults(config);
    FILE* file=fopen(config->filename,"r");
    if(!file) {
        printf("Config file '%s' not found. Using defaults.\n",config->filename);
        return;
    }
    char line[256],key[50],value[50];
    while(fgets(line,sizeof(line),file)) {
        if(line[0]=='#'||line[0]=='\n')continue;
        if(sscanf(line,"%49[^=]=%49s",key,value)==2)parseKeyValue(config,key,value);
    }
    fclose(file);
}
