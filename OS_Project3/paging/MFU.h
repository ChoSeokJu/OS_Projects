//
// Created by hamac on 2020-05-26.
//

#ifndef OS_ASSIGNMENT3_MFU_H
#define OS_ASSIGNMENT3_MFU_H

#include "Paging.h"

class MFU : public Paging {
private:
    vector<pair<int, int>> count;
public:
    void access(int AID);

    void release(int AID);
};


#endif //OS_ASSIGNMENT3_MFU_H
