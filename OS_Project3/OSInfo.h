//
// Created by hamac on 2020-05-26.
//

#ifndef OS_ASSIGNMENT3_OSINFO_H
#define OS_ASSIGNMENT3_OSINFO_H

#include <vector>
#include <set>

#include "paging/Paging.h"
#include "process/Process.h"
#include "utils/Events.h"
#include "memory/PhysicalMemory.h"
#include "scheduler/Scheduler.h"

class OSInfo {
private:
    int clock;
public:
    const int vmemSize, pmemSize, pageSize;

    Events *const events;
    // input 파일에 있는 events를 담고 있는 객체

    PhysicalMemory *pm;
    // 물리메모리

    int page_fault;

    Process *scheduled_process;
    // 스케쥴된 프로세스 (출력용)
    Process *executing_process;
    // 실행할 프로세스

    bool switched;
    // 클럭이 넘어가면서 프로세스가 바뀌었는지 기록하는 변수

    set<int> lock;

    vector<Process *> run_queue;
    // 대기중인 프로세스의 포인터 배열

    vector<Process *> sleep_list;
    // 자고있는 프로세스의 배열

    vector<Process *> iowait;
    // io를 대기하고있는 프로세스의 배열

    static map<int, int> optimal;

    OSInfo(Events *events, int vmemSize, int pmemSize, int pageSize);

    int getClock();
    // 현재 클럭을 리턴함.

    void clockEnd();
    // 하나의 클럭이 끝날때마다 클럭을 하나 증가시킴.
};


#endif //OS_ASSIGNMENT3_OSINFO_H
