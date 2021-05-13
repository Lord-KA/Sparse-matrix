#include "spmatrix.hpp"

int main()
{
    size_t n = 2;
    SPMatrix<int> F(n, n), W(n, n);
    F(0, 0) = 1;
    F(0, 1) = 2;
    F(1, 0) = 3;
    F(1, 1) = 4;
    W(0, 0) = 1;
    W(0, 1) = 0;
    W(1, 0) = 0;
    W(1, 1) = 1;
    std::cout << F << '\n';
    std::cout << W << '\n';
    std::cout << F * W << '\n';

    SPMatrix<int> M1(3, 2), M2(2, 3);
    M1(0, 0) = 1;
    M1(0, 1) = 2;
    M1(1, 0) = 3;
    M1(1, 1) = 4;
    M1(2, 0) = 5;
    M1(2, 1) = 6;

    M2(0, 0) = 7;
    M2(0, 1) = 8;
    M2(0, 2) = 9;
    M2(1, 0) = 10;
    M2(1, 1) = 11;
    M2(1, 2) = 12;

    std::cout << M1 << '\n';
    std::cout << M2 << '\n';
    std::cout << M1 * M2 << '\n';
}
