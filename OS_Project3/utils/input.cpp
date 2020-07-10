//
// Created by hamac on 2020-05-25.
//

#include "input.h"

map<int, vector<pair<string, int>>> getEvents(string dir)
/**
 * INPUT 파일을 파싱함
 * @param dir
 * @return
 */
{
    regex pattern_program("(\\w+)\\s+(\\w+)\\s*");
    regex pattern_input("(\\w+)\\s+(\\w+)\\s+(\\w+)\\s*");
    smatch match;

    map<int, vector<pair<string, int>>> result;
    char buffer[BUFFER_SIZE];

    FILE *fp = fopen((dir + "/input").c_str(), "r");
    if (fp == NULL) throw runtime_error("no input found");

    fgets(buffer, BUFFER_SIZE, fp);
    // 라인 하나 무시

    while (fgets(buffer, BUFFER_SIZE, fp)) {
        string line = buffer;
        if (regex_match(line, match, pattern_program)) {
            int clock = stoi(match[1]);
            string process_name = match[2];

            result[clock].push_back(pair<string, int>{process_name, -1});
        } else if (regex_match(line, match, pattern_input)) {
            if (match[2] == "INPUT") {
                int clock = stoi(match[1]);
                int pid = stoi(match[3]);

                result[clock].push_back(pair<string, int>{"INPUT", pid});
            } else {
                throw runtime_error("invalid input");
            }
        } else {
            throw runtime_error("invalid input");
        }
    }

    fclose(fp);
    return result;
}

tuple<int, int, int> getSizes(string dir)
/**
 * 인풋 파일에서 vmemSize, pmemSize, pageSize를 가져옴
 * @param dir
 * @return
 */
{
    FILE *fp = fopen((dir + "/input").c_str(), "r");
    if (fp == NULL) throw runtime_error("no input found");

    int vmemSize, pmemSize, pageSize;

    fscanf(fp, "%*d\t%d\t%d\t%d\n", &vmemSize, &pmemSize, &pageSize);

    if (!isPowerof2(vmemSize)) throw runtime_error("vmemSize is invalid");
    if (!isPowerof2(pmemSize)) throw runtime_error("pmemSize is invalid");
    if (!isPowerof2(pageSize)) throw runtime_error("pageSize is invalid");

    if ((vmemSize / pageSize) % 4 != 0) throw runtime_error("vmemSize/pageSize is not multiply of 4");
    if ((pmemSize / pageSize) % 4 != 0) throw runtime_error("pmemSize/pageSize is not multiply of 4");

    fclose(fp);
    return make_tuple(vmemSize, pmemSize, pageSize);
}
