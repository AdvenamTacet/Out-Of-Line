#ifndef ARITHMETIC_WRAPPER_H
#define ARITHMETIC_WRAPPER_H

template<typename T>
class fundamental_wrapper {
T value;
public:
  fundamental_wrapper(T const &value) : value(value) {
  }

  operator T &() & {
    return value;
  }
  operator T const &() const & {
    return value;
  }
  operator T &&() && {
    return value;
  }
};

#endif // ARITHMETIC_WRAPPER_H
