//
// Created by hamac on 2020-05-26.
//

#include "LRU.h"

void LRU::access(int AID) {
    // executing process null 아니고 현재 명령어는 MemoryAccess
    PageTable *pt = os_info->executing_process->pt;
    PhysicalMemory *pm = os_info->pm;

    if (pt->isPageExist(AID) && !pt->isValid(AID)) {
        // page가 할당돼있고 Access하려는 page가 valid하지 않다면
        // page fault
        int page_size = pt->getPageNum(AID);

        while (!pm->allocate(AID, page_size)) {
            int deallocated = stack.front();
            stack.erase(stack.begin());
            // 큐에서 꺼내고 삭제함
            setInvalidPageTable(deallocated);
            pm->release(deallocated);
        }
        //물리 메모리에 할당 완료

        pt->setValidBit(AID);

        stack.push_back(AID);
        // ValidBit을 true로 설정하고 stack에 넣음

        os_info->page_fault++;
    } else if (pt->isPageExist(AID) && pt->isValid(AID)) {
        // Page에 frame이 이미 할당돼있을 경우 (물리 메모리에 Access)
        // Access한 AID를 스택에서 제거한 후, 맨 위에 놓음
        auto it = find(stack.begin(), stack.end(), AID);
        stack.erase(it);
        stack.push_back(AID);

        for (auto k: stack) {
            cout << "AID: " << k << endl;
        }
    }
}

void LRU::release(int AID) {
    Paging::release(AID);

    auto it = find(stack.begin(), stack.end(), AID);

    if (it != stack.end())
        stack.erase(it);
}
