#ifndef TIME_HPP
#define TIME_HPP

#include <iostream>

class Time{
    private:
        uint64_t unix_time;

    public:
        // time(); //TODO think if default constructor works this way
        Time(size_t n = 0);
        Time(const Time &other);
        ~Time();
        
        Time operator+(const Time &other) const;
        Time operator-(const Time &other) const;
        Time& operator+=(const Time &other);
        Time& operator-=(const Time &other);


    friend std::ostream& operator<<(std::ostream &out, const Time &T);

    friend std::istream& operator>>(const std::istream &out, Time &T);
};


#endif
