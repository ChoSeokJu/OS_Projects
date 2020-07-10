//
// Created by hamac on 2020-05-24.
//

#include "FCFS.h"

Process *FCFS::schedule()
/**
 * First Come First Serve
 * @return
 */
{
    if (os->executing_process == nullptr) {
        if (!os->run_queue.empty()) return os->run_queue[0];
        else return nullptr;
    } else {
        return os->executing_process;
    }
}