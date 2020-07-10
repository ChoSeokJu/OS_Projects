//
// Created by hamac on 2020-05-26.
//

#include "SampledLRU.h"

void SampledLRU::access(int AID) {
    // executing process null 아니고 현재 명령어는 MemoryAccess
    PageTable *pt = os_info->executing_process->pt;
    PhysicalMemory *pm = os_info->pm;

    if (pt->isPageExist(AID) && !pt->isValid(AID)) {
        // page가 할당돼있고 Access하려는 page가 valid하지 않다면
        // page fault
        int page_size = pt->getPageNum(AID);

        while (!pm->allocate(AID, page_size)) {
            stable_sort(stack.begin(), stack.end(),
                        [](pair<int, pair<bool, unsigned char>> a, pair<int, pair<bool, unsigned char>> b) -> bool {
                            if (a.second.first != b.second.first) return a.second.first < b.second.first;
                            if (a.second.second == b.second.second) return a.first < b.first;
                            return a.second.second < b.second.second;
                        });
            // Reference Byte를 기준으로 내림차순으로 정렬

            for (auto k: stack) {
                cout << "AID: " << k.first << " reference bit: " << ((k.second.first) ? "1" : "0")
                     << " reference byte: "
                     << bitset<8>(k.second.second) << endl;
            }

            int deallocated = stack[0].first;
            // evict할 AID를 지정함

            stack.erase(stack.begin());
            // 큐에서 꺼내고 삭제함

            setInvalidPageTable(deallocated);
            pm->release(deallocated);
        }
        //물리 메모리에 할당 완료

        pt->setValidBit(AID);

        stack.push_back(pair<int, pair<bool, unsigned char>>{AID, pair<bool, unsigned char>{false, 0}});
        // ValidBit을 true로 설정하고 Queue에 넣음

        os_info->page_fault++;
    } else if (pt->isPageExist(AID) && pt->isValid(AID)) {
        // Page에 frame이 이미 할당돼있을 경우 (물리 메모리에 Access)
        for (auto it = stack.begin(); it != stack.end(); it++) {
            if (it->first == AID) {
                it->second.first = true;
                // set reference bit as 1
            }
        }
    }
}

void SampledLRU::release(int AID) {
    Paging::release(AID);

    auto it = stack.begin();

    for (; it != stack.end(); it++) {
        if (it->first == AID) break;
    }

    if (it != stack.end())
        stack.erase(it);
}

void SampledLRU::clock() {
    time++;
    if (time % 8 == 0) {
        for (auto &k: stack) {
            unsigned char reference_bit = (k.second.first) ? 1 : 0;

            k.second.second = ((reference_bit << 7) | (k.second.second >> 1));

            k.second.first = false;
        }
    }
}
