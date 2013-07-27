#ifndef __ELF_64_UTIL_TIME__
#define __ELF_64_UTIL_TIME__

#include <sys/time.h>
#include <string>

using namespace std;

class Time {
public:
    Time ();
    Time (const Time& othertime);
    Time& operator=(const Time& rhs);

    Time& SetNow();

    int Year()   const { return time.tm_year + 1900;}
    int Month()  const { return time.tm_mon;}
    int MDay()   const { return time.tm_mday;}
    int Hour()   const { return time.tm_hour;}
    int Minute() const { return time.tm_min;}
    int Second() const { return time.tm_sec;}
    int MSec()   const { return tv.tv_usec / 1000;}
    int USec()   const { return tv.tv_usec;}

    string Timestamp() const;
    string FileTimestamp() const;
private:
    struct timeval tv;
    struct tm time;
};

#endif
