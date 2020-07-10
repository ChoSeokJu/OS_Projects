#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>

#include "conv.h"
#include "io.h"
#include "structs.h"
#include "const.h"
#include "time.h"


int main(int argc, char **argv) {
    Image *input, *filters, *results; //입력 데이터, 필터, 결과를 저장
    int filter_num, process_num; //필터, 프로세스의 개수

    if (argc > 1) process_num = (int) strtol(argv[1], NULL, 10);
    else process_num = 8;
    //프로세스 수

    getInput(&input, &filters, &filter_num); //필터, 입력 데이터을 입력받음
    const int height_f = filters[0].height; //필터 row의 개수
    const int width_f = filters[0].width; //필터 column의 개수
    const int height_o = (input->height + 2 * PADDING_SIZE - height_f) / STRIDE + 1; //결과 row의 개수
    const int width_o = (input->width + 2 * PADDING_SIZE - width_f) / STRIDE + 1; //결과 column의 개수

    int *process_assign_table = (int *) malloc(sizeof(int) * process_num + 1);
    process_assign_table[0] = 0;
    for (int i = 1; i < process_num + 1; i++) {
        process_assign_table[i] =
                process_assign_table[i - 1] + ((i <= filter_num % process_num) ? 1 : 0) + filter_num / process_num;
    }
    //filter를 각 프로세스에 어떻게 분배할지 계획

    struct timespec start_t, end_t;
    clock_gettime(CLOCK_REALTIME, &start_t);
    //타이머 시작

    for (int i = 0; i < process_num; i++) {
        int start = process_assign_table[i]; //각 프로세스에 할당된 필터의 시작 인덱스
        int end = process_assign_table[i + 1]; //각 프로세스에 할당된 필터의 끝 인덱스
        char filename[BUF_SIZE]; //파일 이름을 임시 저장할 버퍼
        sprintf(filename, "./temp%d.txt", i); //temp{프로세스 인덱스}.txt에 각 프로세스 입력 저장
        FILE *fd = fopen(filename, "w");
        fprintf(fd, "%d %d %d\n", end - start, height_f, width_f);
        for (int f = start; f < end; f++) {
            for (int c = 0; c < INPUT_CHANNEL; c++) {
                for (int h = 0; h < height_f; h++) {
                    for (int w = 0; w < width_f; w++) {
                        fprintf(fd, "%d ", filters[f].image[h][w].channel[c]);
                    }
                    fprintf(fd, "\n");
                }
            }
        }
        fprintf(fd, "%d %d\n", input->height, input->width);
        for (int c = 0; c < INPUT_CHANNEL; c++) {
            for (int h = 0; h < input->height; h++) {
                for (int w = 0; w < input->width; w++) {
                    fprintf(fd, "%d ", input->image[h][w].channel[c]);
                }
                fprintf(fd, "\n");
            }
        }
        //각 프로세스에 입력으로 쓰일 필터, 입력 데이터를 파일로 출력
        fclose(fd);
    }

    for (int process_index = 0; process_index < process_num; process_index++) {
        if (fork() == 0) { //새로운 프로세스 생성
            char f_in[BUF_SIZE];
            char f_out[BUF_SIZE];
            sprintf(f_in, "./temp%d.txt", process_index);
            sprintf(f_out, "./out%d.txt", process_index);
            int in = open(f_in, O_RDONLY);
            int out = open(f_out, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
            dup2(in, 0); //각 프로세스의 standard input을 파일로 대체
            dup2(out, 1); //각 프로세스의 standard ouptut을 파일로 대체
            close(in);
            close(out);
            if (execl("program1", NULL) == -1) {
                perror("error in execl");
            }
            //program1 실행
            exit(0);
        } //자식 프로세스가 실행
    }

    for (int i = 0; i < process_num; i++) {
        wait(NULL);
    }
    //자식 프로세스가 모두 끝날 때까지 대기

    results = (Image *) malloc(sizeof(Image) * filter_num);
    for (int i = 0; i < filter_num; i++) {
        initImage(&results[i], height_o, width_o);
    }
    //결과를 저장할 변수 초기화

    FILE **fd = (FILE **) malloc(sizeof(FILE *) * process_num);
    for (int i = 0; i < process_num; i++) {
        int start = process_assign_table[i];
        int end = process_assign_table[i + 1];
        char filename[BUF_SIZE];
        sprintf(filename, "./out%d.txt", i);
        fd[i] = fopen(filename, "r");

        for (int j = start; j < end; j++) {
            for (int k = 0; k < height_o; k++) {
                for (int l = 0; l < width_o; l++) {
                    fscanf(fd[i], "%d", &results[j].image[k][l].channel[0]);
                }
            }
        }
    }

    clock_gettime(CLOCK_REALTIME, &end_t);
    //타이머 종료

    for (int i = 0; i < filter_num; i++) {
        if (!DEBUG) printResult(&results[i]);
    }
    //결과 출력

    for (int i = 0; i < process_num; i++) {
        int temp;
        fscanf(fd[i], "%d", &temp);
        printf("%d ", temp); //각 프로세스에서 소요된 시간을 출력
        fclose(fd[i]);
        char f_in[BUF_SIZE];
        char f_out[BUF_SIZE];
        sprintf(f_in, "./temp%d.txt", i);
        sprintf(f_out, "./out%d.txt", i);
        remove(f_in);
        remove(f_out);
        //임시로 생성한 파일 삭제
    }

    printf("\n%ld\n", getMilliSec(start_t, end_t));
    //실행 시간 출력
    return 0;
}
