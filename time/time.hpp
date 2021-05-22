#ifndef TIME_HPP
#define TIME_HPP

#include <ctime>
#include <iostream>

#define EPOCH_DIFF 11644482617LL


class Time{
    private:
        uint64_t unix_time;
        std::tm time;

        /*
        struct std::tm {

        int sec;   // seconds of minutes from 0 to 61
        int min;   // minutes of hour from 0 to 59
        int hour;  // hours of day from 0 to 24
        int mday;  // day of month from 1 to 31
        int mon;   // month of year from 0 to 11
        int year;  // year since 1900
        int wday;  // days since sunday         [ Ignored by updater ]
        int yday;  // days since January 1st    [ Ignored by updater ]
        int isdst; // hours of daylight savings time

        }
        */


        void update(std::tm t);
        void update(uint64_t u_t);

    public:
        explicit Time(uint64_t n = 0) { update(n); time.tm_isdst = false; }
        Time(const Time &other) : unix_time(other.unix_time), time(other.time) {time.tm_isdst = false; }
        ~Time() {}
        
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
        bool operator!=(const Time &other) const { return unix_time != other.unix_time; }

        explicit operator uint64_t() const { return unix_time; }

        uint64_t FileTime() const;

        void FileTime(std::ostream &out) const;

        uint64_t getUnixTime() const { return unix_time; }
        void setUnixTime(uint64_t n) { unix_time = n; }

        int getSec()  const { return time.tm_sec; }
        int getMin()  const { return time.tm_min; }
        int getHour() const { return time.tm_hour; }
        int getMDay() const { return time.tm_mday; }
        int getMon()  const { return time.tm_mon; }
        int getYear() const { return time.tm_year + 1900; }
        int getWday() const { return time.tm_wday; }
        int getYday() const { return time.tm_yday; }

        void setSec (int n) { time.tm_sec = n; update(time); }
        void setMin (int n) { time.tm_min = n; update(time); }
        void setHour(int n) { time.tm_hour = n; update(time); }
        void setMDay(int n) { time.tm_mday = n; update(time); }
        void setMon (int n) { time.tm_mon = n; update(time); }
        void setYear(int n) { time.tm_year = n - 1900; update(time); }

    friend Time operator+(const uint64_t n, const Time &other);
    friend Time operator-(const uint64_t n, const Time &other);

    friend std::ostream& operator<<(std::ostream &out, const Time &T);

    friend std::istream& operator>>(std::istream &in, Time &T);
};


#endif
