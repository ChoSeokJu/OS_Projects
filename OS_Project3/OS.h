//
// Created by hamac on 2020-05-25.
//

#ifndef OS_ASSIGNMENT3_OS_H
#define OS_ASSIGNMENT3_OS_H

#include <vector>
#include <cstdio>

#include "scheduler/Scheduler.h"
#include "paging/Paging.h"
#include "process/Process.h"
#include "utils/Events.h"
#include "utils/output.h"
#include "OSInfo.h"

using namespace std;

class Process;

class Scheduler;

class Instruction;

class OS {
private:
    Scheduler *const scheduler;

    Paging *const paging;

    OSInfo *os_info;

    string dir;

    FILE *fp_scheduler;
    FILE *fp_memory;
    // file pointer

    bool isOptimal;

    vector<Process *> createProcess();
    // schedule() 안에서 호출할 함수임

    void optimalPreprocess();

public:
    OS(Scheduler *scheduler, Paging *paging, string dir, bool isOptimal = false);

    ~OS() {
        fclose(fp_scheduler);
        fclose(fp_memory);
    }

    bool checkSleep();
    // OSInfo의 sleep_list에 있는 모든 프로세스의 sleep count를 하나 줄임.
    // sleep count가 0이 되는 프로세스를 대기큐로 옮김.
    // 대기큐로 옮겼다면 true 리턴.

    bool checkIO();
    // os_info.events를 확인해 INPUT 이벤트가 있는지 확인함.

    void checkMemoryAccess();

    Process *schedule();
    // scheduler->schedule() 실행해 프로세스를 얻음
    // nullptr이라면 실행중인 프로세스가 없는 것 or 실행중이던 프로세스가 종료된것
    // preemption까지 고려해 프로세스를 선택함

    void printScheduler();
    // scheduler.txt에 결과 출력.

    void printMemory();
    // memory.txt에 결과 출력.

    void terminateProcess();
    // 만약 process->PC
    // process가 사용하던 메모리를 전부 해제함.
    // Physical Memory에 해당 프로세스가 가진 Allocate id가 있다면 할당을 해제한다.

    bool clock();
    // 클럭을 하나 진행시키고, 해야할 작업이 남아있다면 true, 모든 작업이 끝났다면 false 반환

    void printPageFault();
};


#endif //OS_ASSIGNMENT3_OS_H
