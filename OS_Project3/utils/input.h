//
// Created by hamac on 2020-05-25.
//

#ifndef OS_ASSIGNMENT3_INPUT_H
#define OS_ASSIGNMENT3_INPUT_H

#define BUFFER_SIZE 100

#include "utils.h"

#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <regex>

using namespace std;

map<int, vector<pair<string, int>>> getEvents(string dir);
// parse the "INPUT" file

tuple<int, int, int> getSizes(string dir);
// vmemSize, pmemSize, pageSize을 가져옴.

#endif //OS_ASSIGNMENT3_INPUT_H
