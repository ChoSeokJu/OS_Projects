//
// Created by hamac on 2020-05-24.
//

#include "scheduler/Scheduler.h"
#include "scheduler/FCFS.h"
#include "scheduler/RoundRobin.h"
#include "scheduler/SJF_SA.h"
#include "scheduler/SJF_EA.h"

#include "paging/FIFO.h"
#include "paging/LFU.h"
#include "paging/LRU.h"
#include "paging/MFU.h"
#include "paging/Optimal.h"
#include "paging/SampledLRU.h"

#include "OS.h"

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    Scheduler *scheduler;
    Paging *paging;

    string dir;

    map<string, string> arg = parseArguments(argc, argv);

    // process "dir" argument
    if (arg.find("dir") != arg.end()) {
        dir = arg["dir"];
    } else {
        dir = ".";
    }

    // process "sched" argument
    if (arg.find("sched") != arg.end()) {
        const string sched = arg["sched"];
        if (sched == "fcfs") scheduler = new FCFS();
        else if (sched == "rr") scheduler = new RoundRobin();
        else if (sched == "sjf-simple") scheduler = new SJF_SA;
        else if (sched == "sjf-exponential") scheduler = new SJF_EA;
        else throw runtime_error("invalid sched");
    } else {
        scheduler = new FCFS();
    }

    // process "page" argument
    if (arg.find("page") != arg.end()) {
        const string page = arg["page"];
        if (page == "fifo") paging = new FIFO();
        else if (page == "lru") paging = new LRU();
        else if (page == "lru-sampled") paging = new SampledLRU;
        else if (page == "lfu") paging = new LFU();
        else if (page == "mfu") paging = new MFU();
        else if (page == "optimal") {
            paging = new Optimal();
            OS os_optimal(scheduler, paging, dir, true);
            // if page replacement algorithm is optimal, then run the scheduler once
        } else throw runtime_error("invalid page");
    } else {
        paging = new FIFO();
    }

    OS os(scheduler, paging, dir);

    do {

        os.checkSleep();

        os.checkIO();
        // check if there is a process that needs to be awaken

        Process *process = os.schedule();
        // schedule a process

        if (process != nullptr)
            process->execute();
        // execute the process

        os.checkMemoryAccess();

        if (process != nullptr)
            process->timeQuantum();

        os.printScheduler();
        os.printMemory();
        // print the results

        if (process != nullptr)
            process->increasePC();
        // increase the program counter in each process

        os.terminateProcess();
        // check if scheduled process needs to be terminated

    } while (os.clock());

    os.printPageFault();
    // output the number of page faults

    return 0;
}