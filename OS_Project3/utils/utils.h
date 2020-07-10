//
// Created by hamac on 2020-05-27.
//

#ifndef OS_ASSIGNMENT3_UTILS_H
#define OS_ASSIGNMENT3_UTILS_H

#include <map>
#include <string>
#include <regex>

using namespace std;

map<string, string> parseArguments(int argc, char *argv[]);

bool isPowerof2(int n);

#endif //OS_ASSIGNMENT3_UTILS_H
