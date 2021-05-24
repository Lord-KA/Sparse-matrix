#include "ulint.hpp"

int main() {
    ulint uu1(14), uu2(-15), uu3;
    uu3 = uu1 * uu2;
    uu1.dump(std::cerr);
    uu2.dump(std::cerr);
    uu3.dump(std::cerr);


    return 0;

    ulint u1(ULLONG_MAX, 1), u2(ULLONG_MAX, 1);

    u1.dump(std::cerr);
    u1 -= 1;
    u1.dump(std::cerr);
    std::cerr << '\n';

    u2.dump(std::cerr);
    ulint u3 = u1 - u2;
    u3.dump(std::cerr);
    u3 -= 1;
    u3.dump(std::cerr);
}
