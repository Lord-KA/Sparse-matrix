#include "spmatrix.hpp"

int main()
{
    size_t n = 4;
    SPMatrix<int> F(n, n), W(n, 1);
    F(0, 0) = 12;
    for (size_t w = 0; w < n; ++w)
        W(n, 1) = 1;
    std::cout << F << '\n';
}
