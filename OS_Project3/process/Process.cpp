//
// Created by hamac on 2020-05-25.
//

#include "Process.h"

int Process::_pid = 0;

void Process::initProcess(string dir)
/**
 * 프로세스를 초기화함. dir에 있는 프로그램 인풋을 받아온다
 * @param dir
 */
{
    FILE *fp = fopen((dir + "/" + name).c_str(), "r");
    if (fp == NULL) throw runtime_error("no program input found");

    int line_num;

    fscanf(fp, "%d", &line_num);

    for (int i = 0; i < line_num; i++) {
        int instruction, argument;
        bool islast = (i == line_num - 1);

        fscanf(fp, "%d\t%d", &instruction, &argument);

        if (instruction > 7 || instruction < 0) throw runtime_error("invalid instruction");

        instructions.push_back(new Instruction(instruction, argument, islast, this, os_info));
    }

    fclose(fp);
}

void Process::execute()
/**
 * 프로세스를 한 사이클 실행
 */
{
    (*(burst.end() - 1))++;
    if (PC < instructions.size()) {
        instructions[PC]->execute();
    }
}

void Process::increasePC()
/**
 * Program Counter를 증가시킴 명령어에 따라 다름
 */
{
    int instruction = instructions[PC]->instruction;
    if (PC < instructions.size()) {
        switch (instruction) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 7:
                PC++;
                break;
            case 6:
                if (!busy_waiting) {
                    PC++;
                }
                break;
            default:
                throw runtime_error("invalid instruction");
        }
    }
}

bool Process::isEnd()
/**
 * 프로세스를 종료해야하는지 리턴함
 * @return
 */
{
    return PC == instructions.size();
}

bool Process::isSleepEnd()
/**
 * Sleep 명령이 끝나는지 리턴함
 * @return
 */
{
    return sleep == 0;
}

pair<int, int> Process::getCurrentInstruction()
/**
 * 현재 PC가 가르키는 명령어를 리턴함
 * @return
 */
{
    if (PC < instructions.size())
        return pair<int, int>{instructions[PC]->instruction, instructions[PC]->argument};
    return pair<int, int>{0, 0};
}

void Process::timeQuantum()
/**
 * Time Quantum 관련 작업을 수행
 */
{
    if (execution_time == 10) {
        execution_time = 1;
        if (os_info->executing_process != nullptr && PC != instructions.size() - 1) {
            os_info->run_queue.push_back(os_info->executing_process);
        }
        os_info->executing_process = nullptr;
    }
}
