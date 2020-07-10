//
// Created by hamacojr on 20. 4. 13..
//
#include "structs.h"

void initImage(Image *image, const int height, const int width)
/**
 * 이미지 struct를 초기화
 * @param image
 * @param width
 * @param height
 */
{
    image->height = height;
    image->width = width;
    image->image = (Pixel **) malloc(sizeof(Pixel *) * height);
    for (int i = 0; i < height; i++) {
        image->image[i] = (Pixel *) malloc(sizeof(Pixel) * width);
    }
}

void freeImage(Image *image)
/**
 * 이미지 struct를 해제
 * @param image
 */
{
    for (int i = 0; i < image->height; i++) {
        free(image->image[i]);
    }
    free(image->image);
}