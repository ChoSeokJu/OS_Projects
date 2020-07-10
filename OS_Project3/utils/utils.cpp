//
// Created by hamac on 2020-05-27.
//

#include "utils.h"

map<string, string> parseArguments(int argc, char *argv[])
/**
 * 프로그램의 arguments를 받아 파싱함
 * @param argc
 * @param argv
 * @return
 */
{
    map<string, string> result;

    regex pattern("-(\\w+)=(.+)");
    smatch match;

    for (int i = 1; i < argc; i++) {
        string args = argv[i];
        if (regex_search(args, match, pattern)) {
            if (match.size() != 3) throw runtime_error("invalid argument");
            else result[match[1]] = match[2];
        }
    }
    return result;
}

bool isPowerof2(int n) {
    return (n == (n & -n));
}