#include "arrays.h"
#include <stdlib.h>
#include <stdio.h>

#define INITIAL_HUNTERS 100
#define INITIAL_STARS 100
#define INITIAL_TAXIS 50

bool allocArrays(struct ArrayData* arr) {
    arr->hunterCapacity=INITIAL_HUNTERS;
    arr->starCapacity=INITIAL_STARS;
    arr->taxiCapacity=INITIAL_TAXIS;
    arr->hunterCount=0;
    arr->starCount=0;
    arr->taxiCount=0;
    arr->hunters=(struct Hunter*)malloc(arr->hunterCapacity*sizeof(struct Hunter));
    arr->stars=(struct Star*)malloc(arr->starCapacity*sizeof(struct Star));
    arr->taxis=(struct Taxi*)malloc(arr->taxiCapacity*sizeof(struct Taxi));
    return arr->hunters&&arr->stars&&arr->taxis;
}

void initArrays(struct ArrayData* arr) {
    for(int i=0;i<arr->hunterCapacity;++i){
        arr->hunters[i].alive=false;
        arr->hunters[i].body=NULL;
        arr->hunters[i].dirCalculated=false;
        arr->hunters[i].stunned=false;
        arr->hunters[i].dx=arr->hunters[i].dy=0.0f;
    }
    for(int i=0;i<arr->starCapacity;++i){
        arr->stars[i].alive=false;
        arr->stars[i].body=NULL;
    }
    for(int i=0;i<arr->taxiCapacity;++i){
        arr->taxis[i].alive=false;
        arr->taxis[i].body=NULL;
    }
}

bool reallocHunters(struct ArrayData* arr) {
    int oldCap=arr->hunterCapacity;
    arr->hunterCapacity*=2;
    struct Hunter* new_hunters=(struct Hunter*)realloc(arr->hunters,arr->hunterCapacity*sizeof(struct Hunter));
    if(!new_hunters)return false;
    arr->hunters=new_hunters;
    for(int j=oldCap;j<arr->hunterCapacity;j++){
        arr->hunters[j].alive=false;
        arr->hunters[j].body=NULL;
        arr->hunters[j].dirCalculated=false;
        arr->hunters[j].stunned=false;
        arr->hunters[j].dx=arr->hunters[j].dy=0.0f;
    }
    return true;
}

bool reallocStars(struct ArrayData* arr) {
    int oldCap=arr->starCapacity;
    arr->starCapacity*=2;
    struct Star* new_stars=(struct Star*)realloc(arr->stars,arr->starCapacity*sizeof(struct Star));
    if(!new_stars)return false;
    arr->stars=new_stars;
    for(int j=oldCap;j<arr->starCapacity;j++){
        arr->stars[j].alive=false;
        arr->stars[j].body=NULL;
    }
    return true;
}

bool reallocTaxis(struct ArrayData* arr) {
    int oldCap=arr->taxiCapacity;
    arr->taxiCapacity*=2;
    struct Taxi* new_taxis=(struct Taxi*)realloc(arr->taxis,arr->taxiCapacity*sizeof(struct Taxi));
    if(!new_taxis)return false;
    arr->taxis=new_taxis;
    for(int j=oldCap;j<arr->taxiCapacity;j++){
        arr->taxis[j].alive=false;
        arr->taxis[j].body=NULL;
    }
    return true;
}

void cleanupArrays(struct ArrayData* arr) {
    for(int i=0;i<arr->hunterCount;i++)if(arr->hunters[i].body)delwin(arr->hunters[i].body);
    for(int i=0;i<arr->starCount;i++)if(arr->stars[i].body)delwin(arr->stars[i].body);
    for(int i=0;i<arr->taxiCount;i++)if(arr->taxis[i].body)delwin(arr->taxis[i].body);
    free(arr->hunters);
    free(arr->stars);
    free(arr->taxis);
}

void eraseEntities(struct ArrayData* arr, struct Player* player) {
    werase(player->body);
    for(int i=0;i<arr->hunterCount;i++)if(arr->hunters[i].alive&&arr->hunters[i].body)werase(arr->hunters[i].body);
    for(int i=0;i<arr->starCount;i++)if(arr->stars[i].alive&&arr->stars[i].body)werase(arr->stars[i].body);
}
