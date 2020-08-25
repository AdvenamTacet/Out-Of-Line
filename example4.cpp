#include "out_of_line.hpp"

class X {
public:
    X &operator=(X const &) = delete;
};

using example_t = out_of_line<X, int>;

int main() {
    example_t obj;
}
