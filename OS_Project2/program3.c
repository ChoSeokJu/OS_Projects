//
// Created by hamacojr on 20. 4. 13..
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#include "conv.h"
#include "io.h"
#include "structs.h"
#include "const.h"
#include "time.h"

Image *input, *filters, *results; //입력 데이터, 필터, 결과를 저장
int filter_num, thread_num; //필터, 스레드의 개수
int *thread_assign_table;

void *convThread(void *);

int main(int argc, char **argv) {
    if (argc > 1) thread_num = (int) strtol(argv[1], NULL, 10);
    else thread_num = 8;
    //스레드 수 조정.

    getInput(&input, &filters, &filter_num); //필터, 입력 데이터을 입력받음
    results = (Image *) malloc(sizeof(Image) * filter_num); //스레드로부터 결과를 전달 받기 위해 동적 할당

    thread_assign_table = (int *) malloc(sizeof(int) * thread_num + 1);
    thread_assign_table[0] = 0;
    for (int i = 1; i < thread_num + 1; i++) {
        thread_assign_table[i] =
                thread_assign_table[i - 1] + ((i <= filter_num % thread_num) ? 1 : 0) + filter_num / thread_num;
    }
    //filter를 각 스레드에 어떻게 분배할지 계획

    pthread_t *p_thread = (pthread_t *) malloc(sizeof(p_thread) * thread_num);
    int *thread_index_table = (int *) malloc(sizeof(int) * thread_num);
    long *thread_time_table = (long *) malloc(sizeof(long) * thread_num); //각 스레드의 실행 시간을 저장하기 위한 배열

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    //타이머 시작

    for (int i = 0; i < thread_num; i++) {
        thread_index_table[i] = i;
        pthread_create(&p_thread[i], NULL, convThread, (void *) &thread_index_table[i]);
    }

    for (int i = 0; i < thread_num; i++) {
        long thread_time;
        pthread_join(p_thread[i], (void **) &thread_time); //스레드 종료까지 대기
        thread_time_table[i] = thread_time; //스레드 실행 시간을 저장
    }

    clock_gettime(CLOCK_REALTIME, &end);
    //타이머 끝

    for (int i = 0; i < filter_num; i++) {
        if (!DEBUG) printResult(&results[i]);
    }
    //결과 출력

    for (int i = 0; i < thread_num; i++) {
        printf("%ld ", thread_time_table[i]);
    }
    printf("\n%ld\n", getMilliSec(start, end));
    //실행 시간 출력
    return 0;
}

void *convThread(void *data)
/**
 * 각 스레드에서 실행할 함수
 * @param data
 * @return
 */
{
    struct timespec start_t, end_t;
    clock_gettime(CLOCK_REALTIME, &start_t);
    //타이머 시작

    int start = thread_assign_table[*(int *) data];
    int end = thread_assign_table[*(int *) data + 1];
    for (int i = start; i < end; i++) {
        conv(input, &filters[i], &results[i]);
    }
    //할당받은 필터와 입력데이터로 결과 계산

    clock_gettime(CLOCK_REALTIME, &end_t);
    //타이머 끝
    return (void *) getMilliSec(start_t, end_t);
}