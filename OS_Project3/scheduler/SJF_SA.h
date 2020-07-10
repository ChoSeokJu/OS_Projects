//
// Created by hamac on 2020-05-24.
//

#ifndef OS_ASSIGNMENT3_SJF_SA_H
#define OS_ASSIGNMENT3_SJF_SA_H

#include "./Scheduler.h"

class SJF_SA : public Scheduler {
public:
    Process *schedule();
};


#endif //OS_ASSIGNMENT3_SJF_SA_H
