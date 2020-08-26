/*
 * A a class out_of_line, which is a memory pattern for high performance C++,
 * preserving RAII and keeping only often used class members in cache,
 * with zero memory overhead.
 *
 * The class helps to avoid cache misses.
 */

#ifndef OutOfLine
#define OutOfLine

#include "fundamental_wrapper.hpp"

#include <map>
#include <tuple>

/*
 * Class out_of_line inherits from HotType
 * and has the same size as HotType.
 *
 * ColdType is moved to another memory segment.
 * Cold data is returned by member function "cold".
 */
template<typename HotType, typename ColdType>
class out_of_line : public std::conditional<
std::is_fundamental<HotType>::value, fundamental_wrapper<HotType>, HotType>::type {
    using hot_t = std::conditional<
        std::is_fundamental<HotType>::value, fundamental_wrapper<HotType>, HotType>::type;
    using map_t = std::map<hot_t const *, ColdType>;

    static map_t cold_storage;
public:
    out_of_line() = default;

    out_of_line(HotType const &hot_data, ColdType const &cold_data) : hot_t(hot_data) {
        cold_storage[this] = cold_data;
    }

    out_of_line(out_of_line const &r) : out_of_line(r, r.cold()){
    }

    out_of_line(out_of_line &&r) : hot_t(static_cast<hot_t &&>(r)) {
        cold() = std::move(r.cold());     
    }

    ~out_of_line() {
        cold_storage.erase(this);
    }

    out_of_line &operator=(out_of_line const &r) {
        static_cast<hot_t &>(*this) = r;
        cold() = r.cold();

        return *this;
    }

    out_of_line &operator=(out_of_line &&r) {
        static_cast<hot_t &>(*this) = static_cast<hot_t &&>(r);
        cold() = std::move(r.cold());

        return *this;
    }

    /*
     * Implementation of functions get.
     * Allows structured binding declaration.
     */
    template<size_t Idx>
    auto &get() & {
        if constexpr(Idx == 0) { return static_cast<hot_t &>(*this); }
        if constexpr(Idx == 1) { return cold(); }
    }

    template<size_t Idx>
    auto &get() const& {
        if constexpr(Idx == 0) { return static_cast<hot_t const &>(*this); }
        if constexpr(Idx == 1) { return cold(); }
    }

    template<size_t Idx>
    auto &&get() && {
        if constexpr(Idx == 0) { return static_cast<hot_t &&>(*this); }
        if constexpr(Idx == 1) { return std::move(cold()); }
    }

    /*
     * Functions returning hot and cold data.
     */

    HotType &hot() {
        return static_cast<hot_t &>(*this);
    }

    HotType const &hot() const {
        return static_cast<hot_t const &>(*this);
    }

    ColdType &cold() {
        return cold_storage[this];
    }

    ColdType const &cold() const {
        return cold_storage[this];
    }
};

template<typename HotType, typename ColdType>
out_of_line<HotType, ColdType>::map_t out_of_line<HotType, ColdType>::cold_storage;

namespace std {
    template <typename HotType, typename ColdType>
    struct tuple_size<out_of_line<HotType, ColdType>> : std::integral_constant<size_t, 2> { };

    template <typename HotType, typename ColdType>
    struct tuple_element<0, out_of_line<HotType, ColdType>> {
        using type =std::conditional<
           std::is_fundamental<HotType>::value, fundamental_wrapper<HotType>, HotType>::type;
    };

    template <typename HotType, typename ColdType>
    struct tuple_element<1, out_of_line<HotType, ColdType>> { using type = ColdType; };
}

#endif // OutOfLine
