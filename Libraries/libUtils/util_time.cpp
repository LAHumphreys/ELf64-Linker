#include "util_time.h"
#include <ctime>
#include <sstream>

Time::Time() {
    gettimeofday(&tv,NULL);
    localtime_r(&tv.tv_sec,&time);
}

Time::Time(const Time& rhs) {
    (*this) = rhs;
}

Time& Time::operator=(const Time& rhs) {
    this->tv = rhs.tv;
    this->time = rhs.time;
    return *this;
}

string Time::Timestamp() const {
    stringstream strtime;
    strtime << Year() << "-" << Month() << "-" << MDay();
    strtime << " " << Hour() << ":" << Minute() << ":" << Second();
    strtime << "." << USec();
    return strtime.str();
}
