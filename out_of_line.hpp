/*
 * A a class out_of_line, which is a memory pattern for high performance C++,
 * preserving RAII and keeping only often used class members in cache,
 * with zero memory overhead.
 *
 * The class helps to avoid cache misses.
 */

#ifndef OutOfLine
#define OutOfLine

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
class out_of_line : public HotType {
    static std::map<HotType const *, ColdType> cold_storage;

public:
    out_of_line() = default;

    out_of_line(HotType const &hot_data, ColdType const &cold_data) : HotType(hot_data) {
        cold_storage[this] = cold_data;
    }

    out_of_line(out_of_line const &r) : out_of_line(r, r.cold()){
    }

    out_of_line(out_of_line &&r) : HotType(static_cast<HotType &&>(r)) {
        cold() = std::move(r.cold());     
    }

    ~out_of_line() {
        cold_storage.erase(this);
    }

    out_of_line &operator=(out_of_line const &r) {
        static_cast<HotType &>(*this) = r;
        cold() = r.cold();

        return *this;
    }

    out_of_line &operator=(out_of_line &&r) {
        static_cast<HotType &>(*this) = static_cast<HotType &&>(r);
        cold() = std::move(r.cold());

        return *this;
    }

    /*
     * Implementation of functions get.
     * Allows structured binding declaration.
     */
    template<size_t Idx>
    auto &get() & {
        if constexpr(Idx == 0) { return static_cast<HotType &>(*this); }
        if constexpr(Idx == 1) { return cold(); }
    }

    template<size_t Idx>
    auto &get() const& {
        if constexpr(Idx == 0) { return static_cast<HotType const &>(*this); }
        if constexpr(Idx == 1) { return cold(); }
    }

    template<size_t Idx>
    auto &&get() && {
        if constexpr(Idx == 0) { return static_cast<HotType &&>(*this); }
        if constexpr(Idx == 1) { return std::move(cold()); }
    }

    /*
     * Functions returning cold data.
     */

    ColdType &cold() {
        return cold_storage[this];
    }

    ColdType const &cold() const {
        return cold_storage[this];
    }
};

template<typename HotType, typename ColdType>
std::map<HotType const *, ColdType> out_of_line<HotType, ColdType>::cold_storage;

namespace std {
    template <typename HotType, typename ColdType>
    struct tuple_size<out_of_line<HotType, ColdType>> : std::integral_constant<size_t, 2> { };

    template <typename HotType, typename ColdType>
    struct tuple_element<0, out_of_line<HotType, ColdType>> { using type = HotType; };

    template <typename HotType, typename ColdType>
    struct tuple_element<1, out_of_line<HotType, ColdType>> { using type = ColdType; };
}

#endif // OutOfLine
