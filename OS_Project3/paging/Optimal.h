//
// Created by hamac on 2020-05-26.
//

#ifndef OS_ASSIGNMENT3_OPTIMAL_H
#define OS_ASSIGNMENT3_OPTIMAL_H

#include <algorithm>
#include <climits>
#include "Paging.h"

class Optimal : public Paging {
private:
    vector<pair<int, int>> count;
public:
    void access(int AID);

    void release(int AID);
};


#endif //OS_ASSIGNMENT3_OPTIMAL_H
