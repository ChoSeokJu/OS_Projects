//
// Created by hamac on 2020-05-26.
//

#include "Optimal.h"

void Optimal::access(int AID) {
// executing process null 아니고 현재 명령어는 MemoryAccess
    Process *p = os_info->executing_process;
    PageTable *pt = os_info->executing_process->pt;
    PhysicalMemory *pm = os_info->pm;

    if (pt->isPageExist(AID) && !pt->isValid(AID)) {
        // page가 할당돼있고 Access하려는 page가 valid하지 않다면
        // page fault
        int page_size = pt->getPageNum(AID);

        while (!pm->allocate(AID, page_size)) {
            for (auto &k: count) k.second = INT_MAX;

            for (auto k: OSInfo::optimal) {
                if (k.first > os_info->getClock()) {
                    for (auto it = count.begin(); it != count.end(); it++) {
                        if (it->first == k.second) it->second = min(it->second, k.first - os_info->getClock());
                    }
                }
            }

            stable_sort(count.begin(), count.end(), [](pair<int, int> a, pair<int, int> b) -> bool {
                if (a.second == b.second) return a.first < b.first;
                return a.second > b.second;
            });

            for (auto k: count) {
                cout << "AID: " << k.first << " optimal: " << k.second << endl;
            }

            int deallocated = count[0].first;

            count.erase(count.begin());

            setInvalidPageTable(deallocated);
            pm->release(deallocated);
        }
        //물리 메모리에 할당 완료

        pt->setValidBit(AID);

        count.push_back(pair<int, int>{AID, 0});
        // ValidBit을 true로 설정하고 Queue에 넣음

        os_info->page_fault++;
    } else if (pt->isPageExist(AID) && pt->isValid(AID)) {
        // Page에 frame이 이미 할당돼있을 경우 (물리 메모리에 Access)
    }
}

void Optimal::release(int AID) {
    Paging::release(AID);

    auto it = count.begin();

    for (; it != count.end(); it++) {
        if (it->first == AID) break;
    }

    if (it != count.end())
        count.erase(it);
}
