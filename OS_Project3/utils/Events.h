//
// Created by hamac on 2020-05-25.
//

#ifndef OS_ASSIGNMENT3_EVENTS_H
#define OS_ASSIGNMENT3_EVENTS_H

#include <map>
#include <string>
#include <utility>
#include <vector>
#include "input.h"

using namespace std;

class Events {
private:
    map<int, vector<pair<string, int>>> events;
public:
    Events(string dir) {
        events = getEvents(dir);
    }

    bool hasEvent(int time);
    // 시간 time에 이벤트가 있는지 확인함.

    vector<pair<string, int>> getEvent(int time);

    bool noEventLeft(int clock);
    // time에 발생한 이벤트를 리턴함.
    // 이벤트는 여러개 일 수 있음.
};


#endif //OS_ASSIGNMENT3_EVENTS_H
