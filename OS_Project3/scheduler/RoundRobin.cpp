//
// Created by hamac on 2020-05-24.
//

#include "RoundRobin.h"

Process *RoundRobin::schedule()
/**
 * Round Robin Algorithm
 * @return
 */
{
    if (os->executing_process == nullptr) {
        if (!os->run_queue.empty()) {
            os->run_queue[0]->execution_time = 1;
            return os->run_queue[0];
        } else return nullptr;
    } else {
        os->executing_process->execution_time++;
        return os->executing_process;
    }
}