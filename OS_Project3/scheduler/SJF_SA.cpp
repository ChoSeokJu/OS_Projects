//
// Created by hamac on 2020-05-24.
//

#include "SJF_SA.h"

Process *SJF_SA::schedule()
/**
 * Shortest Job First - Simple Averaging
 * @return
 */
{
    vector<Process *> process_list;

    if (os->executing_process != nullptr) {
        process_list.push_back(os->executing_process);
    }
    process_list.insert(process_list.end(), os->run_queue.begin(), os->run_queue.end());

    stable_sort(process_list.begin(), process_list.end(), [](Process *a, Process *b) -> bool {
        function<double(Process *)> sa = [](Process *p) -> double {
            double result = 0;

            if (p->burst.size() == 1) return 5 - p->burst[0];

            for (auto it = p->burst.begin(); it != p->burst.end() - 1; it++) {
                result += *it;
            }

            return result / ((double) p->burst.size() - 1) - p->burst.back();
        };

        return sa(a) < sa(b);
    });
    // sort entire list of process

    if (!process_list.empty())
        return process_list[0];
    return nullptr;
}
