//
// Created by hamac on 2020-06-22.
//

#ifndef OS_ASSIGNMENT4_STRUCTS_H
#define OS_ASSIGNMENT4_STRUCTS_H

#include <vector>
#include <string>
#include <cmath>

using namespace std;

const int block_size = 1024;

typedef struct inode {
    int id; // inode ID (0 ~ 127)
    string name; // inode name (file name)
    int size; // file size
    int total_block;
    int block[3];

    inode(int id, string name, int size) : id(id), name(name), size(size) {
        block[0] = block[1] = block[2] = 0;
        if (size <= 12 * block_size) {
            block[0] = ceil((double)size / block_size);
        } else if (size <= (12 + 256) * block_size) {
            block[0] = 12;
            block[1] = ceil((double)size / block_size) - 12 + 1;
        } else if (size <= (12 + 256 + 256 * 256) * block_size) {
            block[0] = 12;
            block[1] = 256 + 1;
            block[2] = 1 +
                    (ceil((double)size / block_size) - 12 - 256) +
                    ceil((ceil((double)size / block_size) - 12 - 256) / 256);
        }
        total_block = block[0] + block[1] + block[2];
    }
} inode;

typedef struct dentry {
    string name; // dentry name (directory name)
    dentry *parent; // parent dentry
    vector<dentry *> d_dentry; // a list of children dentries
    vector<inode *> d_inode; // a list of inodes

    dentry() {}

    dentry(string name, dentry* parent) : name(name), parent(parent) {}

} dentry;

#endif //OS_ASSIGNMENT4_STRUCTS_H
