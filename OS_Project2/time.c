//
// Created by hamacojr on 20. 4. 14..
//

#include "time.h"

long getMilliSec(struct timespec start_time, struct timespec end_time)
/**
 * 두개의 timespec를 입력받아 millisecond 단위로 변환해 리턴
 * @param start_time
 * @param end_time
 * @return
 */
{
    return (end_time.tv_sec - start_time.tv_sec) * 1000 + (end_time.tv_nsec - start_time.tv_nsec) / 1000000;
}
