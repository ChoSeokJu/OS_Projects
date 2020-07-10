//
// Created by ChoSeokJu on 2020-05-31.
//

#include "Paging.h"

void Paging::setOSInfo(OSInfo *os_info) {
    this->os_info = os_info;
}

void Paging::setInvalidPageTable(int AID) {
    vector<Process *> p;

    if (os_info->executing_process != nullptr)
        p.push_back(os_info->executing_process); // Scheduled_process 가 아님에 매우 유의!

    p.insert(p.end(), os_info->run_queue.begin(), os_info->run_queue.end());
    p.insert(p.end(), os_info->sleep_list.begin(), os_info->sleep_list.end());
    p.insert(p.end(), os_info->iowait.begin(), os_info->iowait.end());

    for (auto process: p) {
        process->pt->setInvalidBit(AID);
    }
}

void Paging::release(int AID) {
    os_info->executing_process->pt->release(AID);
    os_info->pm->release(AID);
}
