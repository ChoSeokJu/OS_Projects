//
// Created by hamac on 2020-05-24.
//

#include "SJF_EA.h"

Process *SJF_EA::schedule()
/**
 * Shortest Job First - Exponential Averaging
 * @return
 */
{
    vector<Process *> process_list;

    if (os->executing_process != nullptr) {
        process_list.push_back(os->executing_process);
    }
    process_list.insert(process_list.end(), os->run_queue.begin(), os->run_queue.end());

    stable_sort(process_list.begin(), process_list.end(), [](Process *a, Process *b) -> bool {
        function<double(Process *)> ea = [](Process *p) -> double {
            double result = 0;
            int power = 0;

            if (p->burst.size() == 1) return 5 - p->burst[0];

            for (auto it = p->burst.rbegin() + 1; it != p->burst.rend(); it++) {
                if (it == p->burst.rend() - 1)
                    result += pow(1 - 0.6, power++) * (*it);
                else
                    result += 0.6 * pow(1 - 0.6, power++) * (*it);
            }

            return result - p->burst.back();
        };
        // sort entire list of process

        return ea(a) < ea(b);
    });

    if (!process_list.empty())
        return process_list[0];
    return nullptr;
}