//
// Created by hamac on 2020-05-26.
//

#include "OSInfo.h"

map<int, int> OSInfo::optimal;

OSInfo::OSInfo(Events *events, int vmemSize, int pmemSize, int pageSize)
        : events(events), vmemSize(vmemSize), pmemSize(pmemSize), pageSize(pageSize) {
    pm = new PhysicalMemory(pmemSize, pageSize);
    clock = 1;
    page_fault = 0;
    executing_process = nullptr;

    PageTable::_AID = 1;
    Process::_pid = 0;

    for (auto k: optimal) {
        cout << "Clock: " << k.first << " AID:" << k.second << endl;
    }
}

int OSInfo::getClock()
/**
 * 현재 클락을 리턴
 * @return
 */
{
    return clock;
}

void OSInfo::clockEnd()
/**
 * 클락이 끝날때마다 실행해 클락을 증가시킴
 */
{
    clock++;
}