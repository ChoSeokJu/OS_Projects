//
// Created by hamacojr on 20. 4. 13..
//
#include "conv.h"

void ReLU(Image *input)
/**
 *  Adjust ReLU to Image input
 * @param input
 */
{
    for (int i = 0; i < input->height; i++) {
        for (int j = 0; j < input->width; j++) {
            for (int k = 0; k < MAX_CHANNEL; k++) {
                input->image[i][j].channel[k] = MAX(input->image[i][j].channel[k], 0);
            }
        }
    }
}

Pixel getPixelFromImage(const Image *input, const int y, const int x)
/**
 * 이미지로부터 픽셀을 가져옴
 * @param input
 * @param y
 * @param x
 * @return
 */
{
    if ((y < 0 || x < 0) || (y >= input->height || x >= input->width)) {
        return (Pixel) {0, 0, 0};
    }
    return input->image[y][x];
}

int adjustFilter(const Image *input, const Image *filter, const int y, const int x)
/**
 * 입력 데이터에 필터를 적용
 * @param input
 * @param filter
 * @param y
 * @param x
 * @return
 */
{
    int result = 0;
    for (int i = 0; i < filter->height; i++) {
        for (int j = 0; j < filter->width; j++) {
            Pixel tmp = getPixelFromImage(input, y + i - PADDING_SIZE, x + j - PADDING_SIZE);
            for (int k = 0; k < INPUT_CHANNEL; k++) {
                result += tmp.channel[k] * filter->image[i][j].channel[k];
            }
        }
    }
    return result;
}

void conv(const Image *input, const Image *filter, Image *const result)
/**
 * 입력 데이터, 필터로부터 convolution layer, ReLU를 적용하여 result에 저장
 * @param input
 * @param filter
 * @param result
 */
{
    const int output_width = (input->width + 2 * PADDING_SIZE - filter->width) / STRIDE + 1;
    const int output_height = (input->height + 2 * PADDING_SIZE - filter->height) / STRIDE + 1;

    initImage(result, output_height, output_width);
    for (int i = 0; i < output_height; i++) {
        for (int j = 0; j < output_width; j++) {
            result->image[i][j].channel[0] = adjustFilter(input, filter, i, j);
        }
    }
    ReLU(result);
}