//
// Created by hamacojr on 20. 4. 13..
//

#ifndef OS_ASSIGNMENT2_CONV_H
#define OS_ASSIGNMENT2_CONV_H

#include <stdlib.h>
#include "structs.h"
#include "const.h"

void ReLU(Image *input);

Pixel getPixelFromImage(const Image *input, int y, int x);

int adjustFilter(const Image *input, const Image *filter, int y, int x);

void conv(const Image *input, const Image *filter, Image *result);

#endif //OS_ASSIGNMENT2_CONV_H
