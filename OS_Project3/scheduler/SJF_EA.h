//
// Created by hamac on 2020-05-24.
//

#ifndef OS_ASSIGNMENT3_SJF_EA_H
#define OS_ASSIGNMENT3_SJF_EA_H

#include <cmath>
#include "./Scheduler.h"

class SJF_EA : public Scheduler {
public:
    Process *schedule();
};


#endif //OS_ASSIGNMENT3_SJF_EA_H
