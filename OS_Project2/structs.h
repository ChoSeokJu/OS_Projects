//
// Created by hamacojr on 20. 4. 13..
//

#ifndef OS_ASSIGNMENT2_STRUCTS_H
#define OS_ASSIGNMENT2_STRUCTS_H

#include <stdlib.h>
#include "const.h"

typedef struct
/**
 * struct of a single pixel
 */
{
    int channel[MAX_CHANNEL];
} Pixel;

typedef struct
/**
 * struct of array of Pixels
 */
{
    int width;
    int height;
    Pixel **image;
} Image;

void initImage(Image *, int, int);

void freeImage(Image *);

#endif //OS_ASSIGNMENT2_STRUCTS_H
