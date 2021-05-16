#include "time.hpp"


Time Time::operator+(const Time &other) const 
{
    Time result(other);
    result.unix_time += this->unix_time;
    return result;
}

Time Time::operator-(const Time &other) const 
{
    Time result(other);
    result.unix_time -= this->unix_time;
    return result;
}

Time& Time::operator+=(const Time &other)
{
    this->unix_time += other.unix_time;
    return (*this);
}

Time& Time::operator-=(const Time &other)
{
    this->unix_time -= other.unix_time;
    return (*this);
}

Time Time::operator+(const uint64_t n) const {
    Time result(unix_time + n);
    return result;
}

Time Time::operator-(const uint64_t n) const {
    Time result(unix_time - n);
    return result;
}

Time& Time::operator+=(const uint64_t n) {
    unix_time += n;
    return (*this);
}

Time& Time::operator-=(const uint64_t n) {
    unix_time -= n;
    return (*this);
}

Time& Time::operator=(const Time &other) {
    unix_time = other.unix_time;
    return (*this);
}

size_t Time::FileTime() const {
    return unix_time - EPOCH_DIFF;
}

void Time::FileTime(std::ostream &out) const {
    out << Time(unix_time - EPOCH_DIFF);
}

Time operator+(const uint64_t n, const Time &other){
    Time result(other.unix_time + n);
    return result;
}

Time operator-(const uint64_t n, const Time &other){
    Time result(other.unix_time - n);
    return result;
}


std::ostream& operator<<(std::ostream &out, const Time &T){
    std::time_t t(T.unix_time);
    out << std::ctime(&t);
    return out;
}

std::istream& operator>>(std::istream &in, Time &T){
    in >> T.unix_time;
    return in;
}
