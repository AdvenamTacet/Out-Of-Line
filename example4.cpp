#include "out_of_line.hpp"

#include <iostream>

using example_t = out_of_line<uint64_t, uint64_t>;

int main() {
    example_t a(1, 2), b(3, 4);

    std::cout << a << ' ' << a.cold() << std::endl;
    std::swap(a, b);
    std::cout << a << ' ' << a.cold() << std::endl;    

    auto &[a1, a2] = a;
    a2 = 7;
    a.hot() = a2 + 3;

    std::cout << a << ' ' << a.cold() << std::endl;    
}
