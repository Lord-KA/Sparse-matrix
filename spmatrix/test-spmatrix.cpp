#include "spmatrix.hpp"
#include "Matrix.hpp"

#include <gtest/gtest.h>

constexpr size_t n = 2;
constexpr size_t k = 17;

TEST(Multiplication, toEandConst) {
    Matrix<int> M1(n, n), M2(n, n);
    M1(0, 0) = 1;
    M1(0, 1) = 2;
    M1(1, 0) = 3;
    M1(1, 1) = 4;
    M2(0, 0) = 1;
    M2(0, 1) = 0;
    M2(1, 0) = 0;
    M2(1, 1) = 1;
    SPMatrix<int> S1(M1), S2(M2);

    EXPECT_EQ( SPMatrix<int>(M1 * M2), S1 * S2 );
    EXPECT_EQ( SPMatrix<int>(M2 * M1), S2 * S1 );
    EXPECT_EQ( S1 * S1, S1 * S1);
    EXPECT_EQ( S2 * S2, S2 * S2);
    
    EXPECT_EQ( S1 * S2 * n, n * S1 * S2 );
    EXPECT_EQ( S2 * n * S1, S1 * n * S2 );
}

TEST(Multiplication, diffSizeCheckByValue) {
    Matrix<int> M3(3, 2), M4(2, 3);
    M3(0, 0) = 1;
    M3(0, 1) = 2;
    M3(1, 0) = 3;
    M3(1, 1) = 4;
    M3(2, 0) = 5;
    M3(2, 1) = 6;

    M4(0, 0) = 7;
    M4(0, 1) = 8;
    M4(0, 2) = 9;
    M4(1, 0) = 10;
    M4(1, 1) = 11;
    M4(1, 2) = 12;

    SPMatrix<int> S3(M3), S4(M4);

    EXPECT_EQ( SPMatrix<int>(M3 * M4), S3 * S4 );
    EXPECT_EQ( S3 * n * S4, n * S3 * S4);
}

TEST(Multiplication, RandomSquares) {
    for(int i = 0; i < 10; ++i){
        int a = rnd() % 20 + 4;
        Matrix<int> M1(a, a), M2(a, a);
        M1.FillMatrixRandom();
        M2.FillMatrixRandom();
    
        SPMatrix<int> S1(M1), S2(M2);
        EXPECT_EQ( SPMatrix<int>(M1 * M2), S1 * S2 );
        EXPECT_EQ( SPMatrix<int>(M2 * M1), S2 * S1 );
        EXPECT_EQ( SPMatrix<int>(M1 * M1), S1 * S1 );
        EXPECT_EQ( SPMatrix<int>(M2 * M2), S2 * S2 );
        EXPECT_NE( SPMatrix<int>(M1 * M2 * M1), S1 * S2 );
    }
}

TEST(Multiplication, RandomDiffSize) {
    for(int i = 0; i < 10; ++i){
        int a = rnd() % 20 + 3, b = rnd() % 20 + 3, c = rnd() % 20 + 3;
        Matrix<int> M1(a, b), M2(b, c);
        M1.FillMatrixRandom();
        M2.FillMatrixRandom();
    
        SPMatrix<int> S1(M1), S2(M2);
        EXPECT_EQ( SPMatrix<int>(M1 * M2), S1 * S2 );
    }
}


TEST(Summing, RandomDiffSize) {
    for(int i = 0; i < 10; ++i){
        int a = rnd() % 50 + 3, b = rnd() % 50 + 3;
        Matrix<int> M1(a, b), M2(a, b);
        M1.FillMatrixRandom();
        M2.FillMatrixRandom();
    
        SPMatrix<int> S1(M1), S2(M2);
        EXPECT_EQ( SPMatrix<int>(M1 + M2), S1 + S2 );
        EXPECT_EQ( SPMatrix<int>(M2 + M1), S2 + S1 );
        EXPECT_EQ( SPMatrix<int>(M1 + M1), S1 + S1 );
        EXPECT_EQ( SPMatrix<int>(M2 + M2), S2 + S2 );
        EXPECT_NE( SPMatrix<int>(M1 + M1 + M1), S1 + S2 );
    }
}

TEST(Summing, RandomDiffSizeSubraction) {
    for(int i = 0; i < 40; ++i){
        Matrix<int> M1(4, 12), M2(4, 12);
        M1.FillMatrixRandom();
        M2.FillMatrixRandom();
    
        SPMatrix<int> S1(M1), S2(M2);
        EXPECT_EQ( SPMatrix<int>(M1 - M2), S1 - S2 );
        EXPECT_EQ( SPMatrix<int>(M2 - M1), S2 - S1 );
        EXPECT_EQ( SPMatrix<int>(M1 - M1), S1 - S1 );
        EXPECT_EQ( SPMatrix<int>(M2 - M2), S2 - S2 );
        EXPECT_NE( SPMatrix<int>(M1 - M1 + M1), S1 - S2 );
    }
}

TEST(Summing, RandomMultiplByConst) {
    size_t cnst = 19;
    for(int i = 0; i < 40; ++i){
        Matrix<int> M1(4, 12), M2(4, 12);
        M1.FillMatrixRandom();
        M2.FillMatrixRandom();
    
        SPMatrix<int> S1(M1), S2(M2);
        EXPECT_EQ( SPMatrix<int>(M1 + M2 * cnst), S1 + S2 * cnst );
        EXPECT_EQ( SPMatrix<int>(M2 + M1 * cnst), S2 + S1 * cnst );
        EXPECT_EQ( SPMatrix<int>(M1 + M1), S1 + S1 );
        EXPECT_EQ( SPMatrix<int>(-M1), -S1);
        EXPECT_EQ( SPMatrix<int>(M2), +S2);


        EXPECT_EQ( SPMatrix<int>(M2 + 3 * M2 * 2 * cnst), 3 * S2 * 2 * cnst + S2 );
        EXPECT_NE( SPMatrix<int>(M1 + M1 + M1), S1 * 13 + S2 );
    }
}

TEST(Other, Transposing) {
    for(int i = 0; i < 50; ++i){
        Matrix<int> M1(4, 12), M2(4, 12);
        M1.FillMatrixRandom();
        M2.FillMatrixRandom();
    
        SPMatrix<int> S1(M1), S2(M2);

        EXPECT_EQ( SPMatrix<int>(M1.Transposition()), S1.Transpose() );
        EXPECT_EQ( SPMatrix<int>(M2.Transposition()), S2.Transpose() );
        EXPECT_NE( SPMatrix<int>(M1.Transposition()), S2.Transpose() );
    }
}

TEST(Other, Copy) {                     
    for (int i = 0; i < 3; ++i){
        Matrix<int> M1(4, 18);
        M1.FillMatrixRandom();
        Matrix<int> M2(M1);
        M2.FillMatrixRandom();
        SPMatrix<int> S1(M1), S2(M2);
        SPMatrix<int> S3 = S1 + S2;
        Matrix<int> M3 = M1 + M2;
        SPMatrix<int> S4(S1 + S3);
        Matrix<int> M4(M3 + M1);

        EXPECT_EQ(SPMatrix<int>(M3), S3);
        EXPECT_EQ(SPMatrix<int>(M4), S4);
        SPMatrix<int> S5;
        S5 = S1 * 17 + S3;
        Matrix<int> M5;
        M5 = M1 * 17 + M3;
        EXPECT_EQ(SPMatrix<int>(M5), S5);
        S5 += S4;
        M5 = M5 + M4;
        EXPECT_EQ(SPMatrix<int>(M5), S5);
        S5 -= S3;
        M5 = M5 - M3;
        EXPECT_EQ(SPMatrix<int>(M5), S5);
        
        Matrix<int> MM1(12, 23), MM2(23, 19);
        MM1.FillMatrixRandom();
        MM2.FillMatrixRandom();
        SPMatrix<int> SS1(MM1), SS2(MM2);
        SS1 *= SS2;
        MM1 = MM1 * MM2;
        SS1 *= 127 * i;
        MM1 = MM1 * 127 * i;
        EXPECT_EQ(SPMatrix<int>(MM1), SS1);
        EXPECT_NE(SPMatrix<int>(MM1), S1);
        EXPECT_NE(SPMatrix<int>(M4), S2);
    }

}

TEST(Other, Move) {
    for (int i = 0; i < 1; ++i){
        Matrix<int> M1(18, 39), M2, M3, M4;
        M1.FillMatrixRandom();
        M2 = M1;
        M2.FillMatrixRandom();
        SPMatrix<int> S1(M1), S2(M2), S3, S4;
        S3 = std::move(S1);
        S4 = std::move(S2);
        M3 = std::move(M1);
        M4 = std::move(M2);
    
        EXPECT_EQ(SPMatrix<int>(M3), S3);
        EXPECT_EQ(SPMatrix<int>(M4), S4);
        SPMatrix<int> S5(std::move(S3));
        Matrix<int> M5(std::move(M3));
        EXPECT_EQ(SPMatrix<int>(M5), S5);
    }     
}

