//
// Created by hamac on 2020-05-26.
//

#ifndef OS_ASSIGNMENT3_LFU_H
#define OS_ASSIGNMENT3_LFU_H

#include <algorithm>
#include <map>
#include "Paging.h"

using namespace std;

class LFU : public Paging {
private:
    vector<pair<int, int>> count;
public:
    void access(int AID);

    void release(int AID);
};


#endif //OS_ASSIGNMENT3_LFU_H
