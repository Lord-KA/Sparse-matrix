#include "yearfa.hpp"

yearfa::yearfa(uint16_t year){    
    assert(0 <= year && year <= armageddon);
    indiction = year % 15;
    if (!indiction)
        indiction = 15;
    sunRing = year % 28;
    if (!sunRing)
        sunRing = 28;
    moonRing = year % 19;
    if (!moonRing)
        moonRing = 19;
}

void yearfa::setYear(uint16_t year){
    assert(1 <= year && year <= armageddon);
    indiction = year % 15;
    if (!indiction)
        indiction = 15;
    sunRing = year % 28;
    if (!sunRing)
        sunRing = 28;
    moonRing = year % 19;
    if (!moonRing)
        moonRing = 19;
}

int yearfa::getYear() const{
    for (uint16_t year = 1; year < 10000; ++year){
        if ((year % 15 == indiction || (year % 15 == 0 && indiction == 15)) && \
            (year % 28 == sunRing   || (year % 28 == 0 && sunRing == 28)) &&     \
            (year % 19 == moonRing  || (year % 19 == 0 && moonRing == 19)))
                return year;
    }
    assert(true);
    return 0;
}
