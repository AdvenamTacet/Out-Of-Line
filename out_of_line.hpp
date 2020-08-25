#ifndef OutOfLine
#define OutOfLine

#include <map>
#include <iostream>

template<typename HotType, typename ColdType>
class out_of_line : public HotType {
    static std::map<HotType const *, ColdType> cold_storage;

public:
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


    ColdType &cold() {
        return cold_storage[this];
    }

    ColdType const &cold() const {
        return cold_storage[this];
    }
};

template<typename HotType, typename ColdType>
std::map<HotType const *, ColdType> out_of_line<HotType, ColdType>::cold_storage;

#endif // OutOfLine
