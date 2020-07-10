//
// Created by hamac on 2020-05-26.
//

#include "FIFO.h"

void FIFO::access(int AID) {
    // executing process null 아니고 현재 명령어는 MemoryAccess
    PageTable *pt = os_info->executing_process->pt;
    PhysicalMemory *pm = os_info->pm;

    if (pt->isPageExist(AID) && !pt->isValid(AID)) {
        // page가 할당돼있고 Access하려는 page가 valid하지 않다면
        // page fault
        int page_size = pt->getPageNum(AID);

        while (!pm->allocate(AID, page_size)) {
            int deallocated = queue[0];

            queue.erase(queue.begin());
            // 큐에서 꺼내고 삭제함

            setInvalidPageTable(deallocated);
            pm->release(deallocated);
        }
        //물리 메모리에 할당 완료

        pt->setValidBit(AID);

        queue.push_back(AID);
        // ValidBit을 true로 설정하고 Queue에 넣음

        os_info->page_fault++;
    } else if (pt->isPageExist(AID) && pt->isValid(AID)) {
        // Page에 frame이 이미 할당돼있을 경우 (물리 메모리에 Access)
    }
}

void FIFO::release(int AID) {
    Paging::release(AID);

    auto it = find(queue.begin(), queue.end(), AID);

    if (it != queue.end())
        queue.erase(it);
}
