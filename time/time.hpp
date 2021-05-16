#ifndef TIME_HPP
#define TIME_HPP

#include <ctime>
#include <iostream>

#define EPOCH_DIFF 11644482617LL


class Time{
    private:
        uint64_t unix_time;

    public:
        Time(uint64_t n = 0) : unix_time(n) {}
        Time(const Time &other) : unix_time(other.unix_time) {}
        ~Time() {};
        
        Time operator+(const Time &other) const;
        Time operator-(const Time &other) const;
        Time& operator+=(const Time &other);
        Time& operator-=(const Time &other);
        
        Time operator+(const uint64_t n) const;
        Time operator-(const uint64_t n) const;
        Time& operator+=(const uint64_t n);
        Time& operator-=(const uint64_t n);

        Time& operator=(const Time &other);
        
        bool operator==(const Time &other) const { return unix_time == other.unix_time; }

        explicit operator uint64_t() const { return unix_time; }

        uint64_t FileTime() const;

        void FileTime(std::ostream &out) const;

        uint64_t getUnixTime() const { return unix_time; }
        void setUnixTime(uint64_t n) { unix_time = n; }

    friend Time operator+(const uint64_t n, const Time &other);
    friend Time operator-(const uint64_t n, const Time &other);

    friend std::ostream& operator<<(std::ostream &out, const Time &T);

    friend std::istream& operator>>(std::istream &in, Time &T);
};


#endif
