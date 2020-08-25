#include "out_of_line.hpp"

#include <iostream>

using example_t = out_of_line<std::string, std::string>;
using example2_t = out_of_line<std::string, uint64_t>;
int main() {
    example_t a("a_hot", "a_cold"), b("b_hot", "b_cold");

    std::cout << a << ' ' << a.cold() << std::endl;
    std::cout << b << ' ' << b.cold() << std::endl;
    std::swap(a, b);
    std::cout << "Swaped." << std::endl;
    std::cout << a << ' ' << a.cold() << std::endl;
    std::cout << b << ' ' << b.cold() << std::endl;

    example2_t c("c_hot", 1);
    std::cout << std::endl << "Snd type:" << std::endl;
    std::cout << c << ", and cold value: " << c.cold() << std::endl;
}
