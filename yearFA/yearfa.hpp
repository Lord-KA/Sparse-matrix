#ifndef YEARFA_HPP
#define YEARFA_HPP

#include <iostream>
#include <cassert>

static constexpr uint16_t armageddon = 15 * 28 * 19;

class yearfa{
    private:
        uint16_t indiction;     // from 1 to 15
        uint16_t sunRing;       // from 1 to 28
        uint16_t moonRing;      // from 1 to 19

    public:
        explicit yearfa(uint16_t n = 0);
        yearfa(uint16_t indiction, uint16_t sunRing, uint16_t moonRing) : indiction(indiction), sunRing(sunRing), moonRing(moonRing) {}
        //yearfa(const yearfa &other) indiction(other.in)

        ~yearfa() = default;
    
        //yearfa& operator=( const yearfa &other ) { indiction = other.indiction; sunRing = other.sunRing; moonRing = other.moonRing; return (*this); }
        bool operator==(const yearfa &other) const { return indiction == other.indiction && sunRing == other.sunRing && moonRing == other.moonRing; }
        bool operator!=(const yearfa &other) const { return indiction != other.indiction || sunRing != other.sunRing || moonRing != other.moonRing; }

        yearfa operator+( const yearfa &other) const { yearfa result(getYear() + other.getYear()); return result; }
        yearfa operator-( const yearfa &other) const { yearfa result(getYear() - other.getYear()); return result; }

        int getIndiction() const { return indiction; }
        int getSunRing()   const { return sunRing; }
        int getMoonRing()  const { return moonRing; }
        int getYear()   const;
        int getYearAC() const { return getYear() - 5508; }

        void setIndiction(uint16_t n) { assert(1 <= n && n <= 15); indiction = n; }
        void setSunRing(  uint16_t n) { assert(1 <= n && n <= 28); sunRing = n; }
        void setMoonRing( uint16_t n) { assert(1 <= n && n <= 19); moonRing = n; }
        void setYear(     uint16_t n);
        void setYearAC(   uint16_t n) { setYear(n + 5508); }     // set by year according to Christ
        

        friend std::ostream& operator<<(std::ostream &out, const yearfa &y) { out << y.getYear(); return out; }
        friend std::istream& operator>>(std::istream &in, yearfa &y) { y.setYear(in.get()); return in; }

};


#endif
