//
// Created by ChoSeokJu on 2020-05-31.
//

#ifndef OS_ASSIGNMENT3_PAGING_H
#define OS_ASSIGNMENT3_PAGING_H

#include <vector>
#include "../OSInfo.h"

class OSInfo;

using namespace std;

class Paging {
protected:
    OSInfo *os_info;
public:
    void setOSInfo(OSInfo *os_info);

    void setInvalidPageTable(int AID);

    virtual void access(int AID) = 0;

    virtual void release(int AID) = 0;

    virtual void clock() {}
};


#endif //OS_ASSIGNMENT3_PAGING_H
