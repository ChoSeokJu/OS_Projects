//
// Created by hamac on 2020-05-26.
//

#ifndef OS_ASSIGNMENT3_FIFO_H
#define OS_ASSIGNMENT3_FIFO_H

#include <algorithm>
#include "Paging.h"

using namespace std;

class FIFO : public Paging {
private:
    vector<int> queue;
public:
    void access(int AID);

    void release(int AID);
};


#endif //OS_ASSIGNMENT3_FIFO_H
