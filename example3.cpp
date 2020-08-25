#include "out_of_line.hpp"

#include <tuple>
#include <vector>
#include <iostream>

using example_t = out_of_line<std::string, std::string>;
using tuple_t = std::tuple<std::string, std::string>;
using tuple_ref = std::tuple<std::string &, std::string &>;

int main() {
    example_t obj("hot", "cold");

    auto &[rhot, rcold] = obj;
    std::cout << rhot << ' ' << rcold << std::endl;
    rhot = "hot_new";
    rcold = "cold_new";

    std::cout << obj << ' ' << obj.cold() << std::endl;

    example_t const cobj("chot", "ccold");

    auto const &[chot, ccold] = cobj;
    std::cout << chot << ' ' << ccold << std::endl;

    auto [hot, cold] = std::move(obj);
    std::cout << "New: " << hot << ' ' << cold << std::endl;
    std::cout << "Old: " << obj << ' ' << obj.cold() << std::endl;

    std::vector<example_t> objs;
    for(uint32_t i : {0, 3, 7, 10}) {
        objs.emplace_back(std::to_string(i), std::to_string(i+1));
    }

    for(auto &[hot, cold] : objs) {
        cold += "~" + hot;
        hot = "0";
    }

    std::cout << "Vector after modification:" << std::endl;
    for(auto const &[hot, cold] : objs) {
        std::cout << hot << ' ' << cold << std::endl;
    }


}
