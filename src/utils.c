#include "utils.h"
#include <stdlib.h>

int randInt(int min, int max) {
    return min + rand() % (max - min + 1);
}
