#include "time.hpp"

// time::time() : unix_time(0){};

Time::Time(size_t n) : unix_time(n) {}; 

Time::Time(const Time &other) : unix_time(other.unix_time) {};

Time::~Time(){};

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

std::ostream& operator<<(std::ostream &out, const Time &T){ /*
    size_t h = (T.unix_time % (60 * 60 * 24)) / 60 ;
    size_t m = (T.unix_time % (60 * 60)) / 60;
    size_t s = (T.unix_time % 60);
    out << "hour: " << h << " minute: " << m << " second: " << s << '\n';
    return out;*/
    out << std::ctime;
}
