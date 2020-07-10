//
// Created by hamac on 2020-05-24.
//

#ifndef OS_ASSIGNMENT3_SCHEDULER_H
#define OS_ASSIGNMENT3_SCHEDULER_H

#include <vector>

#include "../OSInfo.h"
#include "../process/Process.h"

class OSInfo;

class Process;

class Scheduler {
protected:
    OSInfo *os;
public:
    void setOSInfo(OSInfo *os) {
        this->os = os;
    }

    virtual Process *schedule() = 0;
    // 이번 클럭에 실행해야하는 프로세스를 선택함.
};

#endif //OS_ASSIGNMENT3_SCHEDULER_H
