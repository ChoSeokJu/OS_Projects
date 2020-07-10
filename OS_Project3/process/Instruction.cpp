//
// Created by hamac on 2020-05-25.
//

#include "Instruction.h"

void Instruction::execute()
/**
 * 명령어를 수행
 */
{
    switch (instruction) {
        case 0:
            memoryAllocation();
            break;
        case 1:
            memoryAccess();
            break;
        case 2:
            memoryRelease();
            break;
        case 3:
            nonMemory();
            break;
        case 4:
            process->burst.push_back(0);
            sleep();
            break;
        case 5:
            process->burst.push_back(0);
            IOwait();
            break;
        case 6:
            lock();
            break;
        case 7:
            unlock();
            break;
        default:
            throw runtime_error("invalid instruction");
    }
}

void Instruction::memoryAllocation() {
    if (os_info->vmemSize / os_info->pageSize < argument ||
        os_info->pmemSize / os_info->pageSize < argument)
        throw runtime_error("invalid memory size");

    process->pt->latestAID = process->pt->allocate(argument);
}

void Instruction::memoryAccess() {
    process->pt->latestAID = argument;
}

void Instruction::memoryRelease() {
    process->pt->latestAID = argument;
}

void Instruction::nonMemory() {
    // nothing special
}

void Instruction::sleep() {
    if (!islast && argument > 0) {
        process->sleep = argument;

        os_info->sleep_list.push_back(process);
        // Sleep list에 프로세스 추가

        os_info->executing_process = nullptr;
        // 실행중인 프로세스를 없앰
    }
}

void Instruction::IOwait() {
    if (!islast) {
        os_info->iowait.push_back(process);
        // IOwait Queue에 프로세스 추가

        os_info->executing_process = nullptr;
        // 실행중인 프로세스를 없앰
    }
}

void Instruction::lock() {
    if (os_info->lock.find(argument) == os_info->lock.end()) {
        // lock가 돼있지 않다면
        os_info->lock.insert(argument);
        process->busy_waiting = false;
    } else {
        // lock이 돼있다면
        process->busy_waiting = true;
    }
}

void Instruction::unlock() {
    auto it = os_info->lock.find(argument);
    if (it != os_info->lock.end()) {
        os_info->lock.erase(it);
    }
}

