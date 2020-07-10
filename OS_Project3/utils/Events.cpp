//
// Created by hamac on 2020-05-25.
//

#include "Events.h"

bool Events::hasEvent(const int time)
/**
 * INPUT 파일의 time clock에 이벤트가 있는지 확인
 * @param time
 * @return
 */
{
    return !(events.find(time) == events.end());
}

vector<pair<string, int>> Events::getEvent(const int time)
/**
 * time clock에 있는 이벤트를 리턴
 * @param time
 * @return
 */
{
    if (!hasEvent(time))
        throw runtime_error("there is no event");
    return events[time];
}

bool Events::noEventLeft(int clock)
/**
 * 인풋만 남아있을 때 인풋이 유효한 인풋인지 확인해야함.
 * @param clock
 * @return
 */
{
    bool result = true;
    for (auto k: events) {
        if (k.first > clock) {
            result = false;
        }
        // 하나라도 실행해야할 event가 남아있다면 끝나지 않음.
    }
    return result;
}
