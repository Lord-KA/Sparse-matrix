#include "yearfa.hpp"

#include <random>
#include <string>
#include <gtest/gtest.h>


std::mt19937 rnd(179);

TEST(Basic, operators) {
    for (int i = 0; i < 1000; ++i){
        yearfa Y1(rnd() % 3000 + 2), Y2(rnd() % 3000 + 2), Y3;
        Y3 = Y1 + Y2;
        yearfa Y4(Y1 + Y2);
        EXPECT_EQ(Y3, Y4);
        std::stringstream out;
        out << Y1 << Y2 << Y3 << Y4;
    }
}

TEST(Basic, getSetTime) {
    for (int i = 0; i < 1000; ++i){
        yearfa Y1(rnd() % 7000 + 1), Y2(rnd() % 7000 + 1), Y3;
        Y3 = Y2;
        yearfa Y4(Y1);
        EXPECT_EQ(Y3, Y2);
        EXPECT_EQ(Y1, Y4);
        Y1.getIndiction();
        Y1.getSunRing();
        Y1.getMoonRing();
        Y1.getYear();
        Y1.getYearAC();
        
        Y1.setYear(7160);
        EXPECT_EQ(Y1.getYearAC(), 1652);
        EXPECT_EQ(Y1.getYear(), 7160);
        Y2.setYearAC(1652);
        EXPECT_EQ(Y2.getYear(), 7160);
        uint16_t q = rnd() % 2000 + 10;
        Y3.setYearAC(q);
        EXPECT_EQ(Y3.getYearAC(), q);
    }
}
