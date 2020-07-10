//
// Created by hamac on 2020-05-25.
//

#ifndef OS_ASSIGNMENT3_PHYSICALMEMORY_H
#define OS_ASSIGNMENT3_PHYSICALMEMORY_H

#include <vector>
#include <cmath>

using namespace std;

class Node {
public:
    int AID, size;

    bool leaf;

    Node *left, *right;

    Node() {
        leaf = false;
        AID = -1;
        left = right = nullptr;
    }
};

class PhysicalMemory {
private:
    Node *tree;

    Node *constructMemory(int depth);

    bool isValidBlock(Node *n);

    bool allocate(Node *n, int AID, int target_size);

    bool release(Node *n, int AID);

    void getMemory(Node *n, vector<int> *result);

public:
    PhysicalMemory(int pmemSize, int pageSize);

    bool allocate(int AID, int target_size);
    // 물리 메모리에 AID를 target_size 만큼 할당한다

    bool release(int AID);
    // 물리 메모리에서 해당 AID를 해제한다

    vector<int> getMemory();
    // 물리 메모리 출력용
};


#endif //OS_ASSIGNMENT3_PHYSICALMEMORY_H
