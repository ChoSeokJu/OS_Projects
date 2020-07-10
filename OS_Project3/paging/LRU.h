//
// Created by hamac on 2020-05-26.
//

#ifndef OS_ASSIGNMENT3_LRU_H
#define OS_ASSIGNMENT3_LRU_H

#include <algorithm>
#include "Paging.h"

class LRU : public Paging {
private:
    vector<int> stack;
public:
    void access(int AID);

    void release(int AID);
};


#endif //OS_ASSIGNMENT3_LRU_H
