//
// Created by ChoSeokJu on 2020-05-29.
//

#ifndef OS_ASSIGNMENT3_OUTPUT_H
#define OS_ASSIGNMENT3_OUTPUT_H

#include <cstdio>
#include "../OSInfo.h"

void outputScheduler_printf(OSInfo *os_info);

void outputScheduler(FILE *fp_scheduler, OSInfo *os_info);

void outputMemory_printf(OSInfo *os_info);

void outputMemory(FILE *fp_scheduler, OSInfo *os_info);

#endif //OS_ASSIGNMENT3_OUTPUT_H
