//
// Created by ChoSeokJu on 2020-05-29.
//

#include "output.h"

void outputScheduler_printf(OSInfo *os_info)
/**
 * 스케줄러 결과를 화면에 출력
 * @param os_info
 */
{
    Process *p = os_info->scheduled_process;
    printf("[%d Cycle] Scheduled Process: ", os_info->getClock());
    if (p != nullptr && os_info->switched) {
        printf("%d %s\n",
               p->pid,
               p->name.c_str());
    } else {
        printf("None\n");
    }

    printf("Running Process: ");
    if (p != nullptr) {
        printf("Process#%d running code %s line %d(op %d, arg %d) \n",
               p->pid,
               p->name.c_str(),
               p->PC + 1,
               p->instructions[p->PC]->instruction,
               p->instructions[p->PC]->argument);
    } else {
        printf("None\n");
    }


    printf("RunQueue: ");
    if (os_info->run_queue.empty()) {
        printf("Empty");
    } else {
        for (auto k: os_info->run_queue) {
            printf("%d(%s) ", k->pid, k->name.c_str());
        }
    }
    printf("\n");

    printf("SleepList: ");
    if (os_info->sleep_list.empty()) {
        printf("Empty");
    } else {
        for (auto k: os_info->sleep_list) {
            printf("%d(%s) ", k->pid, k->name.c_str());
        }
    }
    printf("\n");

    printf("IOWait List: ");
    if (os_info->iowait.empty()) {
        printf("Empty");
    } else {
        for (auto k: os_info->iowait) {
            printf("%d(%s) ", k->pid, k->name.c_str());
        }
    }
    printf("\n");

//    vector<Process *> k;
//
//    if (os_info->executing_process != nullptr)
//        k.push_back(os_info->executing_process); // Scheduled_process 가 아님에 매우 유의!
//
//    k.insert(k.end(), os_info->run_queue.begin(), os_info->run_queue.end());
//    k.insert(k.end(), os_info->sleep_list.begin(), os_info->sleep_list.end());
//    k.insert(k.end(), os_info->iowait.begin(), os_info->iowait.end());
//
//    sort(k.begin(), k.end(), [](Process *a, Process *b) -> bool {
//        return a->pid < b->pid;
//    });

//    for (auto p: k) {
//        double result = 0;
//        double power = 0;
//
//
//        for (auto it = p->burst.rbegin() + 1; it != p->burst.rend(); it++) {
//            if (it == p->burst.rend() - 1)
//                result += pow(1 - 0.6, power++) * (*it);
//            else
//                result += 0.6 * pow(1 - 0.6, power++) * (*it);
//        }
//
//        if (p->burst.size() == 1) result = 5;
//
//        cout << "pid: " << p->pid << " burst: " << result - p->burst.back() << endl;
//    }
    printf("\n");
}

void outputScheduler(FILE *fp, OSInfo *os_info)
/**
 * 스케줄러 결과를 파일로 출력함
 * @param fp
 * @param os_info
 */
{
    Process *p = os_info->scheduled_process;
    fprintf(fp, "[%d Cycle] Scheduled Process: ", os_info->getClock());
    if (p != nullptr && os_info->switched) {
        fprintf(fp, "%d %s\n",
                p->pid,
                p->name.c_str());
    } else {
        fprintf(fp, "None\n");
    }

    fprintf(fp, "Running Process: ");
    if (p != nullptr) {
        fprintf(fp, "Process#%d running code %s line %d(op %d, arg %d) \n",
                p->pid,
                p->name.c_str(),
                p->PC + 1,
                p->instructions[p->PC]->instruction,
                p->instructions[p->PC]->argument);
    } else {
        fprintf(fp, "None\n");
    }


    fprintf(fp, "RunQueue: ");
    if (os_info->run_queue.empty()) {
        fprintf(fp, "Empty");
    } else {
        for (auto k: os_info->run_queue) {
            fprintf(fp, "%d(%s) ", k->pid, k->name.c_str());
        }
    }
    fprintf(fp, "\n");

    fprintf(fp, "SleepList: ");
    if (os_info->sleep_list.empty()) {
        fprintf(fp, "Empty");
    } else {
        for (auto k: os_info->sleep_list) {
            fprintf(fp, "%d(%s) ", k->pid, k->name.c_str());
        }
    }
    fprintf(fp, "\n");

    fprintf(fp, "IOWait List: ");
    if (os_info->iowait.empty()) {
        fprintf(fp, "Empty");
    } else {
        for (auto k: os_info->iowait) {
            fprintf(fp, "%d(%s) ", k->pid, k->name.c_str());
        }
    }
    fprintf(fp, "\n");

    fprintf(fp, "\n");
}

void outputMemory_printf(OSInfo *os_info) {

}

void outputMemory(FILE *fp, OSInfo *os_info)
/**
 * 메모리 명령 수행 결과를 출력
 * @param fp
 * @param os_info
 */
{
    vector<Process *> p;

    if (os_info->executing_process != nullptr)
        p.push_back(os_info->executing_process); // Scheduled_process 가 아님에 매우 유의!

    p.insert(p.end(), os_info->run_queue.begin(), os_info->run_queue.end());
    p.insert(p.end(), os_info->sleep_list.begin(), os_info->sleep_list.end());
    p.insert(p.end(), os_info->iowait.begin(), os_info->iowait.end());

    sort(p.begin(), p.end(), [](Process *a, Process *b) -> bool {
        return a->pid < b->pid;
    });
    // pid 를 기준으로 오름차순 정렬
    Process *process = os_info->scheduled_process;

    if (process != nullptr) {
        int PC = process->PC;
        int instruction = process->instructions[PC]->instruction;

        string func;

        if (instruction >= 0 && instruction <= 2) {
            if (instruction == 0) func = "ALLOCATION";
            if (instruction == 1) func = "ACCESS";
            if (instruction == 2) func = "RELEASE";

            fprintf(fp, "[%d Cycle] Input : Pid [%d] Function [%s] Alloc ID [%d] Page Num[%d]\n",
                    os_info->getClock(),
                    process->pid,
                    func.c_str(),
                    process->pt->latestAID,
                    process->pt->getPageNum(process->pt->latestAID)
            );

        } else if (instruction >= 3 && instruction <= 7) {
            if (instruction == 3) func = "NONMEMORY";
            if (instruction == 4) func = "SLEEP";
            if (instruction == 5) func = "IOWAIT";
            if (instruction == 6) func = "LOCK";
            if (instruction == 7) func = "UNLOCK";

            fprintf(fp, "[%d Cycle] Input : Pid [%d] Function [%s]\n",
                    os_info->getClock(),
                    process->pid,
                    func.c_str()
            );
        }
    } else {
        fprintf(fp, "[%d Cycle] Input : Function [NO-OP]\n",
                os_info->getClock());
    }

    fprintf(fp, "%-30s", ">> Physical Memory : ");
    vector<int> pm = os_info->pm->getMemory();
    for (int i = 0; i < pm.size(); i++) {
        if (i % 4 == 0)
            fprintf(fp, "|");
        if (pm[i] == -1)
            fprintf(fp, "-");
        else
            fprintf(fp, "%d", pm[i]);
    }
    fprintf(fp, "|\n");

    for (auto process: p) {
        fprintf(fp, ">> pid(%d) %-20s", process->pid, "Page Table(AID) : ");

        vector<int> vm = process->pt->getPages();

        for (int i = 0; i < vm.size(); i++) {
            if (i % 4 == 0)
                fprintf(fp, "|");

            if (vm[i] == -1)
                fprintf(fp, "-");
            else
                fprintf(fp, "%d", vm[i]);
        }
        fprintf(fp, "|\n");

        fprintf(fp, ">> pid(%d) %-20s", process->pid, "Page Table(Valid) : ");

        vector<int> valid_bit = process->pt->getValidBit();

        for (int i = 0; i < valid_bit.size(); i++) {
            if (i % 4 == 0)
                fprintf(fp, "|");

            if (valid_bit[i] != -1)
                fprintf(fp, "%d", valid_bit[i]);
            else
                fprintf(fp, "-");
        }

        fprintf(fp, "|\n");
    }

    fprintf(fp, "\n");
}