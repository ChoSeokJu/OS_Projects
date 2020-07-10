#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "conv.h"
#include "io.h"
#include "structs.h"
#include "const.h"
#include "time.h"

int main(int argc, char **argv) {
    Image *input, *filters, *results; //입력 데이터, 필터, 결과를 저장
    int filter_num; //필터의 개수

    getInput(&input, &filters, &filter_num); //필터, 입력 데이터을 입력받음
    if (filter_num == 0) {
        printf("0");
        return 0;
    } //필터의 개수가 0일 경우 중료
    results = (Image *) malloc(sizeof(Image) * filter_num);

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    //타이머 시작

    for (int i = 0; i < filter_num; i++) {
        conv(input, &filters[i], &results[i]);
        //convolution layer에 인풋을 넣어 결과를 얻음
    }

    clock_gettime(CLOCK_REALTIME, &end);
    //타이머 종료

    for (int i = 0; i < filter_num; i++) {
        if (!DEBUG) printResult(&results[i]);
    }
    //결과 출력

    printf("%ld\n", getMilliSec(start, end));
    //실행 시간 출력

    return 0;
}
