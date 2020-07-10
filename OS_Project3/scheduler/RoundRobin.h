//
// Created by hamac on 2020-05-24.
//

#ifndef OS_ASSIGNMENT3_ROUNDROBIN_H
#define OS_ASSIGNMENT3_ROUNDROBIN_H

#include "./Scheduler.h"

class RoundRobin : public Scheduler {
public:
    Process *schedule();
};


#endif //OS_ASSIGNMENT3_ROUNDROBIN_H
