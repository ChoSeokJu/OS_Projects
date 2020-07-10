//
// Created by hamacojr on 20. 4. 13..
//

#include "io.h"

void getImageInput(Image *image)
/**
 * 사용자로부터 이미지를 입력받음
 * @param image
 */
{
    int temp;
    for (int i = 0; i < INPUT_CHANNEL; i++) {
        for (int j = 0; j < image->height; j++) {
            for (int k = 0; k < image->width; k++) {
                scanf("%d", &temp);
                image->image[j][k].channel[i] = temp;
            }
        }
    }
}

void printResult(Image *image)
/**
 * 이미지를 출력
 * @param image
 */
{
    for (int j = 0; j < image->height; j++) {
        for (int k = 0; k < image->width; k++) {
            printf("%d ", image->image[j][k].channel[0]);
        }
        printf("\n");
    }
    printf("\n");
}


void getInput(Image **input, Image **filters, int *filter_num)
/**
 * 입력 데이터, 필터를 입력받음
 * @param input 
 * @param filters
 * @param filter_num
 */
{
    int filter_height, filter_width;
    int input_height, input_width;

    scanf("%d %d %d", filter_num, &filter_height, &filter_width);
    *filters = (Image *) malloc(sizeof(Image) * (*filter_num));

    for (int i = 0; i < *filter_num; i++) {
        //initialize filters
        initImage(&(*filters)[i], filter_height, filter_width);
        getImageInput(&(*filters)[i]);
    }
    //필터를 입력받음.

    scanf("%d %d", &input_height, &input_width);
    *input = (Image *) malloc(sizeof(Image));

    initImage(*input, input_height, input_width);
    getImageInput(*input);
    //3채널의 인풋을 입력받음.
}