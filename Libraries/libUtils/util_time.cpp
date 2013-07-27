#include "util_time.h"
#include <ctime>
#include <sstream>

Time::Time() {
    SetNow();
}

Time::Time(const Time& rhs) {
    (*this) = rhs;
}

Time& Time::operator=(const Time& rhs) {
    this->tv = rhs.tv;
    this->time = rhs.time;
    return *this;
}

Time& Time::SetNow() {
    gettimeofday(&tv,NULL);
    localtime_r(&tv.tv_sec,&time);
    return *this;
}

string Time::Timestamp() const {
    stringstream strtime;
    strtime << Year() << "-" << Month() << "-" << MDay();
    strtime << " " << Hour() << ":" << Minute() << ":" << Second();
    strtime << "." << USec();
    return strtime.str();
}

string Time::FileTimestamp() const {
    stringstream strtime;
    strtime << Year() << "-" << Month() << "-" << MDay();
    strtime << "_" << Hour() << ":" << Minute() << ":" << Second();
    strtime << "-" << USec();
    return strtime.str();
}
