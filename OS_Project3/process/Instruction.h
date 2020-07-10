//
// Created by hamac on 2020-05-25.
//

#ifndef OS_ASSIGNMENT3_INSTRUCTION_H
#define OS_ASSIGNMENT3_INSTRUCTION_H

#include "../OSInfo.h"
#include "Process.h"

class Process;

class OSInfo;

class Instruction {
private:
    Process *const process;
    // 명령어가 소속된 프로그램의 정보

    OSInfo *const os_info;
    // OS의 정보


    void memoryAllocation();

    void memoryAccess();

    void memoryRelease();

    void nonMemory();

    void sleep();

    void IOwait();

    void lock();

    void unlock();

public:
    const int instruction;
    // 명령어의 번호

    const int argument;
    // 명령어에 전달된 인자

    const bool islast;
    // 명령어가 프로세스의 마지막 명령어인지
    // 마지막 명령어일 경우 Sleep, IOwait 등의 명령을 실행하지 않음.
    // 프로세스가 할당받은 물리 메모리를 모두 반환해야함.

    Instruction(int instruction, int argument, bool islast, Process *process, OSInfo *os_info)
            : instruction(instruction), argument(argument), islast(islast), process(process),
              os_info(os_info) {
    }

    void execute();
};


#endif //OS_ASSIGNMENT3_INSTRUCTION_H
