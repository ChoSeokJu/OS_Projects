//
// Created by hamac on 2020-05-24.
//

#ifndef OS_ASSIGNMENT3_FCFS_H
#define OS_ASSIGNMENT3_FCFS_H

#include "./Scheduler.h"

class FCFS : public Scheduler {
public:
    Process *schedule();
};


#endif //OS_ASSIGNMENT3_FCFS_H
