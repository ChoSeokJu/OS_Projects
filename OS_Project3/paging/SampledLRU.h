//
// Created by hamac on 2020-05-26.
//

#ifndef OS_ASSIGNMENT3_SAMPLEDLRU_H
#define OS_ASSIGNMENT3_SAMPLEDLRU_H

#include <vector>
#include <bitset>

#include "Paging.h"

using namespace std;

class SampledLRU : public Paging {
private:
    int time;

    vector<pair<int, pair<bool, unsigned char>>> stack;
    // AID, Reference bit, Reference Bytes
public:
    SampledLRU() : time(0) {}

    void access(int AID);

    void release(int AID);

    void clock();
};


#endif //OS_ASSIGNMENT3_SAMPLEDLRU_H
