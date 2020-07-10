//
// Created by hamac on 2020-05-25.
//

#include "OS.h"

OS::OS(Scheduler *scheduler, Paging *paging, string dir, bool isOptimal)
        : scheduler(scheduler), paging(paging), dir(dir), isOptimal(isOptimal) {

    fp_scheduler = fopen((dir + "/scheduler.txt").c_str(), "w");
    if (fp_scheduler == NULL) throw runtime_error("fail to create schedule file");

    fp_memory = fopen((dir + "/memory.txt").c_str(), "w");
    if (fp_memory == NULL) throw runtime_error("fail to create memory file");

    tuple<int, int, int> sizes = getSizes(dir);
    os_info = new OSInfo(new Events(dir), get<0>(sizes), get<1>(sizes), get<2>(sizes));

    scheduler->setOSInfo(os_info);
    paging->setOSInfo(os_info);

    if (isOptimal) optimalPreprocess();
}

Process *OS::schedule()
/**
 * get process from run queue
 * @return
 */
{
    os_info->switched = false;
    // 프로세스가 바뀌는지 체크하는 변수

    vector<Process *> created_process = createProcess();

    Process *new_process = scheduler->schedule();

    for (auto it = os_info->run_queue.begin(); it != os_info->run_queue.end(); it++) {
        if (*it == new_process) {
            os_info->run_queue.erase(it--);
        }
    }
    // RunQueue에서 스케쥴 받은 프로세스를 제거함.

    if (os_info->executing_process == nullptr) {
        os_info->switched = true;
        // 실행중이던 프로세스가 없음
    } else if (os_info->executing_process != new_process) {
        // 프로세스가 바뀌었을 경우
        os_info->switched = true;
        if (!os_info->executing_process->isEnd()) {
            os_info->run_queue.push_back(os_info->executing_process);
            // Running Queue 로 다시 돌아감
        }
    }

    os_info->scheduled_process = os_info->executing_process = new_process;
    return new_process;
}

bool OS::checkSleep()
/**
 * decrease the sleep count of the processes in the sleep queue
 * push the process back to run queue
 * @return
 */
{
    bool result = false;

    for (auto it = os_info->sleep_list.begin(); it != os_info->sleep_list.end(); it++) {
        (*it)->sleep--;
        // decrease sleep count

        if ((*it)->isSleepEnd()) {
            result = true;
            os_info->run_queue.push_back(*it);
            os_info->sleep_list.erase(it);
            it--;
        }
        // push back to run queue
    }

    return result;
}

bool OS::checkIO()
/**
 * check IO for processes in wait queue
 * @return
 */
{
    bool result = false;

    if (os_info->events->hasEvent(os_info->getClock())) {
        for (auto k: os_info->events->getEvent(os_info->getClock())) {
            if (k.first == "INPUT") {
                int pid = k.second;

                for (auto it = os_info->iowait.begin(); it != os_info->iowait.end(); it++) {
                    if ((*it)->pid == pid) {
                        os_info->run_queue.push_back(*it);
                        os_info->iowait.erase(it);
                        it--;
                    }
                }
                // push back to run queue if such pid exist
            }
        }
    }
    return result;
}

vector<Process *> OS::createProcess()
/**
 * 프로세스를 생성하는 함수
 * @return
 */
{
    vector<Process *> result;

    if (os_info->events->hasEvent(os_info->getClock())) {
        for (const auto &k: os_info->events->getEvent(os_info->getClock())) {
            string name;
            if (k.second == -1) {
                // if the event is not "INPUT", create new process and push it to run queue
                name = k.first;
                auto *new_process = new Process(os_info, dir, name, os_info->vmemSize, os_info->pageSize);
                result.push_back(new_process);
                os_info->run_queue.push_back(new_process);
            }
        }
    }

    return result;
}

void OS::printScheduler()
/**
 * print the result to scheduler.txt
 */
{
    outputScheduler_printf(os_info);
    outputScheduler(fp_scheduler, os_info);
}

void OS::printMemory()
/**
 * print the result to memory.txt
 */
{
    outputMemory_printf(os_info);
    outputMemory(fp_memory, os_info);
}

bool OS::clock()
/**
 * 클럭이 끝날때마다 한번씩 실행하는 함수
 * @return
 */
{
    bool result = false;

    if (!os_info->run_queue.empty()) result = true;
    if (!os_info->sleep_list.empty()) result = true;
    if (!os_info->iowait.empty()) result = true;

    if (os_info->executing_process != nullptr) result = true;
    if (os_info->executing_process != nullptr && !os_info->executing_process->isEnd()) result = true;

    if (!os_info->events->noEventLeft(os_info->getClock())) result = true;

    os_info->clockEnd();
    // 클럭을 하나 증가시킴

    return result;
}

void OS::terminateProcess()
/**
 * 프로세스를 종료하는 함수
 */
{
    if (os_info->executing_process != nullptr && os_info->executing_process->isEnd()) {
        // delete os_info->scheduled_process;

        set<int> AIDs = os_info->executing_process->pt->getAID();
        // page table에서 allocate된 모든 AID가져옴

        for (int AID: AIDs) {
            paging->release(AID);
        }
        // 물리메모리에서 모두 release 함

        os_info->executing_process = nullptr;
    }
}

void OS::checkMemoryAccess()
/**
 * 메모리 접근명령이 있었는지 확인하고 수행하는 함수
 */
{
    if (os_info->scheduled_process != nullptr && !os_info->scheduled_process->busy_waiting) {
        int instruction = os_info->scheduled_process->getCurrentInstruction().first;
        int AID = os_info->scheduled_process->getCurrentInstruction().second;

        if (!isOptimal) {
            if (instruction == 1) {
                paging->access(AID);
            }
            if (instruction == 2) {
                paging->release(AID);
            }
            paging->clock();
        } else {
            if (instruction == 1) {
                OSInfo::optimal.insert(pair<int, int>{os_info->getClock(), AID});
            }
        }
    }
}

void OS::printPageFault()
/**
 * 페이지 폴트 개수를 출력함
 */
{
    fprintf(fp_memory, "page fault = %d", os_info->page_fault);
}

void OS::optimalPreprocess()
/**
 * Optimal 페이징 교체 알고리즘을 사용하기 전에 사용해줘야하는 함수
 */
{

    do {

        checkSleep();

        checkIO();

        Process *p = schedule();

        if (p != nullptr)
            p->execute();

        checkMemoryAccess();

        if (p != nullptr)
            p->increasePC();

        terminateProcess();

    } while (clock());

}
