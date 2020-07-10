//
// Created by hamac on 2020-05-25.
//

#ifndef OS_ASSIGNMENT3_PAGETABLE_H
#define OS_ASSIGNMENT3_PAGETABLE_H

#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

class PageTable {
    int num_pages;
    int *pages;
    bool *valid_bit;

    map<int, int> pageNum;

    int getNewAID();

public:

    static int _AID;
    int latestAID;

    PageTable() {}

    PageTable(int vmemSize, int pageSize);

    void initPageTable(int vmemSize, int pageSize);

    int allocate(int target_size);

    bool release(int AID);
    // 해당 AID를 해제한다

    void setValidBit(int AID);
    // 해당 AID가 할당돼있다면 ValidBit을 true로 설정한다

    void setInvalidBit(int AID);
    // 해당 AID의 ValidBit을 false로 설정한다

    vector<int> getPages();
    // 페이지 출력용

    vector<int> getValidBit();
    // ValidBit 출력용

    int getPageNum(int AID);
    // AID에 해당하는 페이지 개수를 리턴한다

    bool isValid(int AID);
    // 해당 AID가 valid한지 리턴한다

    bool isPageExist(int AID);
    // 해당 AID가 있는지 확인한다

    set<int> getAID();
};


#endif //OS_ASSIGNMENT3_PAGETABLE_H
