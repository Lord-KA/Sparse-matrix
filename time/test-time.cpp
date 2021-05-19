#include "time.hpp"

#include <random>
#include <string>
#include <gtest/gtest.h>


std::mt19937 rnd(179);

TEST(Basic, operators) {
    for (int i = 0; i < 100; ++i){
        Time t1(rnd()), t2(rnd());
        t1 += t2;
        t1 += rnd();
        t2 += rnd();
        t2 -= t1;
        Time t3(t1), t4(t2), t5, t6;
        t5 = t1;
        t6 = t2;

        EXPECT_EQ( t1, t3 );
        EXPECT_EQ( t5, t3 );
        EXPECT_EQ( t2, t4 );
        EXPECT_EQ( t6, t2 );
        int64_t n = rnd();
        EXPECT_EQ( t1 + n, t3 + n );
        n = rnd();
        EXPECT_EQ( t4 - n, t6 - n );
        EXPECT_EQ( t1 - t2 - n, t3 - t6 - n );
        t4.FileTime();
        std::stringstream out;
        t4.FileTime(out);
        int64_t k = t1.getUnixTime();
        t2.setUnixTime(k);
        EXPECT_EQ( t1, t2 );
        out >> t6;
        EXPECT_NE( t1, t6 );
    }
}

TEST(Basic, GetSetTime) {
    for (int i = 0; i < 100; ++i){
        Time t1(rnd()), t2(rnd());
        t1.setSec(rnd() % 61);
        t2.setMin(rnd() % 61);
        t1.setHour(rnd() % 25);
        t2.setMDay(rnd() % 32);
        t1.setMon(rnd() % 13);
        t2.setYear(rnd());
   }

}
