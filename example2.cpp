#include "out_of_line.hpp"

#include <iostream>

class Fast {
public:
    char a = 'a', b = 'b';
};

class Slow {
public:
    char c = 'c', d = 'd';
};

class Example1 : public out_of_line<Fast, Slow> {
public:
    using out_of_line<Fast, Slow>::out_of_line;

    char &get_c() {
        return cold().c;
    }

    char const &get_c() const {
        return cold().c;
    }

    char &get_d() {
        return cold().d;
    }

    char const &get_d() const {
        return cold().d;
    }

    int32_t a_plus_c() const {
        return a + get_c();
    }
};

class Example2 : public out_of_line<std::string, std::string> {
public:
    using out_of_line<std::string, std::string>::out_of_line;
};

std::ostream &operator<<(std::ostream &os, Example2 const &obj) { 
    return os << static_cast<std::string const &>(obj) << ", " << obj.cold();
}


int main() {
    std::cout << "Example1" << std::endl;

    Example1 obj1({}, {});
    std::cout << obj1.a << ", " << obj1.get_c() << ", " << obj1.a_plus_c() << std::endl;

    obj1.get_c() = 'z';
    std::cout << obj1.a_plus_c() << std::endl << std::endl;

    std::cout << "Example2" << std::endl;
    Example2 obj2("hot", "cold");
    std::cout << obj2 << std::endl;
}
