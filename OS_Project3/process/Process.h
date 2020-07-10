//
// Created by hamac on 2020-05-25.
//

#ifndef OS_ASSIGNMENT3_PROCESS_H
#define OS_ASSIGNMENT3_PROCESS_H

#include <cstdio>
#include <vector>
#include <string>

#include "../OSInfo.h"

#include "Instruction.h"

#include "PageTable.h"

using namespace std;

class OSInfo;

class Instruction;

class Process {
private:

    OSInfo *const os_info;

    void initProcess(string dir);
    // processinfo에 담겨있는 프로세스 이름을 기반으로
    // 프로세스의 파일에서 명령어를 읽어와 초기화함.
    // 각 명령어는 이 프로세스의 processinfo를 가지고 있음.

public:
    vector<Instruction *> instructions;
    // 명령어를 담고있는 배열

    static int _pid;
    const int pid;
    // 프로세스의 pid

    const string name;
    // 인풋으로 주어진 프로세스의 이름

    PageTable *pt;

    int execution_time;
    // time quantum 관련 변수
    // scheduler가 RR일때만 사용
    // execution time이 10이 되면 다시 0으로 만들어야함.

    int PC;
    // 현재 실행중인 코드 인덱스

    int sleep;
    // sleep가 얼마나 남았는지 확인.

    bool busy_waiting;

    vector<int> burst;

    Process(OSInfo *os_info, string dir, string name, int vmemSize, int pageSize)
            : pid(_pid++), os_info(os_info), name(name), sleep(0) {
        pt = new PageTable(vmemSize, pageSize);
        execution_time = 0;
        PC = 0;
        sleep = 0;
        busy_waiting = false;
        burst.push_back(0);

        initProcess(dir);
    }

    void execute();
    // 현재 명령어를 실행함

    void increasePC();
    // 명령어를 실행한 후 PC를 증가시키는 함수

    bool isEnd();
    //마지막 명령어를 실행한 이후라면 true, 아니면 false

    bool isSleepEnd();
    // Sleep가 끝난는지 리턴하는 함수.

    pair<int, int> getCurrentInstruction();

    void timeQuantum();
};

#endif //OS_ASSIGNMENT3_PROCESS_H
