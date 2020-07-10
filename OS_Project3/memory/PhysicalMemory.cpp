//
// Created by hamac on 2020-05-25.
//

#include "PhysicalMemory.h"

PhysicalMemory::PhysicalMemory(int pmemSize, int pageSize) {
    int num_frames = pmemSize / pageSize;
    tree = constructMemory(log2(num_frames) + 1);
}

Node *PhysicalMemory::constructMemory(int depth)
/**
 * 물리 메모리에 대한 내용을 저장하는 이진트리를 구축함
 * @param depth
 * @return
 */
{
    Node *n = new Node();
    n->size = pow(2, depth - 1);

    if (depth == 1) {
        n->leaf = true;
        return n;
    }

    n->left = constructMemory(depth - 1);
    n->right = constructMemory(depth - 1);
    return n;
}

bool PhysicalMemory::allocate(int AID, int target_size)
/**
 * 해당 AID를 target_size만큼 할당
 * @param AID
 * @param target_size
 * @return
 */
{
    int num = target_size;
    int allocate_size = 1;

    while (num >>= 1)
        allocate_size <<= 1;

    if (target_size - allocate_size > 0) allocate_size *= 2;

    return allocate(tree, AID, allocate_size);
}

bool PhysicalMemory::allocate(Node *n, int AID, int target_size) {
    if (n->AID != -1) return false;

    if (target_size == n->size) {
        bool valid = isValidBlock(n);
        if (valid) n->AID = AID;
        return valid;
    }

    bool left = allocate(n->left, AID, target_size);
    bool right = false;
    if (!left) right = allocate(n->right, AID, target_size);

    return left || right;
}

bool PhysicalMemory::isValidBlock(Node *n) {
    if (n->AID != -1) return false;
    if (n->leaf) return n->AID == -1;

    return isValidBlock(n->left) && isValidBlock(n->right);
}

bool PhysicalMemory::release(int AID)
/**
 * 해당 AID를 해제함
 * @param AID
 * @return
 */
{
    return release(tree, AID);
}

bool PhysicalMemory::release(Node *n, int AID) {
    if (n->AID == AID) {
        n->AID = -1;
        return true;
    }
    if (n->leaf)
        return false;

    return release(n->left, AID) || release(n->right, AID);
}

void PhysicalMemory::getMemory(Node *n, vector<int> *result) {
    if (n->leaf) {
        result->push_back(n->AID);
        return;
    }

    if (n->AID != -1) {
        for (int i = 0; i < n->size; i++)
            result->push_back(n->AID);
        return;
    }

    getMemory(n->left, result);
    getMemory(n->right, result);
}

vector<int> PhysicalMemory::getMemory()
/**
 * 물리 메모리 전체 내용을 리턴
 * @return
 */
{
    vector<int> result;
    getMemory(tree, &result);
    return result;
}

