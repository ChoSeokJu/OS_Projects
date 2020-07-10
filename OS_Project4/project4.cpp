//
// Created by hamac on 2020-06-22.
//

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <regex>

#include "structs.h"

using namespace std;

const int max_block_num = 1024 - 51; // # of blocks - # of reserved blocks
const int max_inode_num = 128;

const char *user = "2018147580";
const char *err_msg = "error\n";

dentry *root;

dentry *current_dentry;

set<int> inode_table;

string getCurrentPath(dentry *curr) {
    if (curr == root) return string("/");
    if (curr->parent == root) return string("/") + curr->name;
    else return getCurrentPath(curr->parent) + "/" + curr->name;
}

int totalUsingBlock(dentry *d) {
    int result = 0;

    for (auto inode: d->d_inode) {
        result += inode->total_block;
    }

    for (auto dentry: d->d_dentry) {
        result += totalUsingBlock(dentry);
    }

    return result;
}

void printSpaceLeft() {
    printf("Now you have ...\n");
    printf("%d / 973 (blocks)\n", max_block_num - totalUsingBlock(root));
}

vector<string> prompt() {
    char buffer[128];
    vector<string> result;
    regex pattern("\\S+");

    string prompt;

    printf("%s:%s$ ", user, getCurrentPath(current_dentry).c_str());

    fgets(buffer, sizeof(buffer), stdin);
    prompt = buffer;

    smatch m;
    while (regex_search(prompt, m, pattern)) {
        result.push_back(m[0]);
        prompt = m.suffix();
    }

    return result;
}

void ls(vector<string> input) {
    auto d_dentry = current_dentry->d_dentry;
    auto d_inode = current_dentry->d_inode;

    sort(d_inode.begin(), d_inode.end(), [](inode *a, inode *b) {
        return a->id < b->id;
    });
    // 오름차순으로 정렬

    for (auto dentry: d_dentry) {
        printf("%s ", dentry->name.c_str());
    }

    for (auto inode: d_inode) {
        printf("%s ", inode->name.c_str());
    }

    printf("\n");
}

void cd(vector<string> input) {
    if (input.size() != 2) {
        printf("%s", err_msg);
        return;
    }

    string path = input[1];
    bool relative = (path[0] != '/');

    dentry *target_dentry = (relative) ? current_dentry : root;

    regex pattern("[^/]+");

    smatch m;
    while (regex_search(path, m, pattern)) {
        if (m[0] == ".") {
            target_dentry = target_dentry;
        } else if (m[0] == "..") {
            target_dentry = target_dentry->parent;
        } else {
            bool found = false;
            for (auto dentry: target_dentry->d_dentry) {
                if (m[0] == dentry->name) {
                    target_dentry = dentry;
                    found = true;
                }
            }
            if (!found) {
                printf("%s", err_msg);
                return;
            }
        }
        path = m.suffix();
    }

    current_dentry = target_dentry;
}

void mkdir(vector<string> input) {
    if (input.size() == 1) {
        printf("%s", err_msg);
        return;
    }

    for (int i = 1; i < input.size(); i++) {
        string name = input[i];

        for (auto dentry: current_dentry->d_dentry) {
            if (dentry->name == name) {
                printf("%s", err_msg);
                return;
            }
        }
    }
    // 이미 있는 폴더인지 확인한다.

    for (int i = 1; i < input.size(); i++) {
        current_dentry->d_dentry.push_back(new dentry(input[i], current_dentry));
    }
}

void delete_dir(dentry *d) {
    for (auto k: d->d_inode) {
        inode_table.erase(k->id);
        delete k;
    }

    for (auto k: d->d_dentry) {
        delete_dir(k);
        delete k;
    }
}

void rmdir(vector<string> input) {
    if (input.size() == 1) {
        printf("%s", err_msg);
        return;
    }

    for (int i = 1; i < input.size(); i++) {
        bool found = false;
        for (auto dentry: current_dentry->d_dentry) {
            if (dentry->name == input[i]) found = true;
        }

        if (!found) {
            printf("%s", err_msg);
            return;
        }
    }
    // 해당 디렉토리가 있는지 확인한다

    for (int i = 1; i < input.size(); i++) {
        for (auto it = current_dentry->d_dentry.begin(); it != current_dentry->d_dentry.end(); ++it) {
            if ((*it)->name == input[i]) {
                delete_dir(*it);
                current_dentry->d_dentry.erase(it);
                it--;
            }
        }
    }

    printSpaceLeft();
}

void mkfile(vector<string> input) {
    if (input.size() != 3) {
        printf("%s", err_msg);
        return;
    }

    string name = input[1];
    int size = stoi(input[2]);

    for (auto k: current_dentry->d_inode) {
        if (k->name == name) {
            printf("%s", err_msg);
            return;
        }
    }
    // 이미 있는 파일인지 확인

    if (inode_table.size() > max_inode_num) {
        printf("%s", err_msg);
        return;
    }
    // inode의 개수가 꽉 찼는지 확인

    int id = 0;
    for (int i = 0; i < max_inode_num; i++) {
        if (inode_table.find(i) == inode_table.end()) {
            id = i;
            break;
        }
    }
    // id를 할당하는 작업

    inode *new_inode = new inode(id, name, size);

    if (totalUsingBlock(root) + new_inode->total_block > max_block_num) {
        printf("%s", err_msg);
        delete new_inode;
        return;
    }

    inode_table.insert(id);

    current_dentry->d_inode.push_back(new_inode);

    printSpaceLeft();
}

void rmfile(vector<string> input) {
    if (input.size() == 1) {
        printf("%s", err_msg);
        return;
    }

    for (int i = 1; i < input.size(); i++) {
        bool found = false;
        for (auto inode: current_dentry->d_inode) {
            if (inode->name == input[i]) found = true;
        }

        if (!found) {
            printf("%s", err_msg);
            return;
        }
    }
    // 모든 파일이 있는지 확인한다

    for (int i = 1; i < input.size(); i++) {
        for (auto it = current_dentry->d_inode.begin(); it != current_dentry->d_inode.end(); ++it) {
            if ((*it)->name == input[i]) {
                inode_table.erase((*it)->id);
                delete *it;
                current_dentry->d_inode.erase(it);
                it--;
            }
        }
    }

    printSpaceLeft();
}

void inode_func(vector<string> input) {
    if (input.size() != 2) {
        printf("%s", err_msg);
        return;
    }

    string name = input[1];

    inode *target;

    bool found = false;
    for (auto k: current_dentry->d_inode) {
        if (k->name == name) {
            target = k;
            found = true;
        }
    }
    if (!found) {
        printf("%s", err_msg);
        return;
    }
    // 있는 파일인지 확인

    printf("ID: %d\nName: %s\nSize: %d (bytes)\n", target->id, target->name.c_str(), target->size);
    printf("Direct blocks: %d\n", target->block[0]);
    printf("Single indirect blocks: %d\n", target->block[1]);
    printf("Double indirect blocks: %d\n", target->block[2]);
}

void exit_func(vector<string> input) {
    exit(0);
}

const vector<string> instr = {
        "ls",
        "cd",
        "mkdir",
        "rmdir",
        "mkfile",
        "rmfile",
        "inode",
        "exit"
};

const vector<function<void(vector<string> input)>> instr_func = {
        ls,
        cd,
        mkdir,
        rmdir,
        mkfile,
        rmfile,
        inode_func,
        exit_func
};

int main() {
    root = new dentry("", nullptr);
    root->parent = root;
    current_dentry = root;

    while (true) {
        vector<string> input = prompt();

        if (!input.empty()) {
            for (int i = 0; i < instr.size(); i++) {
                if (instr[i] == input[0]) {
                    instr_func[i](input);
                }
            }
        }
    }
}