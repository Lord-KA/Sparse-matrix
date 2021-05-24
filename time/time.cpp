#include "time.hpp"

void Time::update(std::tm t)
{ 
    time = t;
    unix_time = std::mktime(&time);
    assert(unix_time != -1);
}

void Time::update(uint64_t u_t)
{
    unix_time = u_t;
    time_t t = static_cast<time_t>(u_t);
    std::tm *tmp = std::gmtime(&t);
    assert(tmp);
    time = *tmp;
}


Time Time::operator+(const Time &other) const 
{
    Time result(other);
    result.unix_time += this->unix_time;
    result.update(result.unix_time);
    return result;
}

Time Time::operator-(const Time &other) const 
{
    Time result(other);
    result.unix_time -= this->unix_time;
    result.update(result.unix_time);
    return result;
}

Time& Time::operator+=(const Time &other)
{
    this->unix_time += other.unix_time;
    update(unix_time);
    return (*this);
}

Time& Time::operator-=(const Time &other)
{
    this->unix_time -= other.unix_time;
    update(unix_time);
    return (*this);
}

Time Time::operator+(const uint64_t n) const {
    Time result(unix_time + n);
    result.update(result.unix_time);
    return result;
}

Time Time::operator-(const uint64_t n) const {
    Time result(unix_time - n);
    result.update(result.unix_time);
    return result;
}

Time& Time::operator+=(const uint64_t n) {
    unix_time += n;
    update(unix_time);
    return (*this);
}

Time& Time::operator-=(const uint64_t n) {
    unix_time -= n;
    update(unix_time);
    return (*this);
}

Time& Time::operator=(const Time &other) {
    unix_time = other.unix_time;
    time = other.time;
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
    result.update(result.unix_time);
    return result;
}

Time operator-(const uint64_t n, const Time &other){
    Time result(other.unix_time - n);
    result.update(result.unix_time);
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
