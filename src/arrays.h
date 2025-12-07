#ifndef ARRAYS_H
#define ARRAYS_H

#include "hunter.h"
#include "star.h"
#include "taxi.h"
#include <stdbool.h>

struct ArrayData {
    struct Hunter* hunters;
    struct Star* stars;
    struct Taxi* taxis;
    int hunterCapacity;
    int starCapacity;
    int taxiCapacity;
    int hunterCount;
    int starCount;
    int taxiCount;
};

bool allocArrays(struct ArrayData* arr);
void initArrays(struct ArrayData* arr);
bool reallocHunters(struct ArrayData* arr);
bool reallocStars(struct ArrayData* arr);
bool reallocTaxis(struct ArrayData* arr);
void cleanupArrays(struct ArrayData* arr);
void eraseEntities(struct ArrayData* arr, struct Player* player);

#endif
