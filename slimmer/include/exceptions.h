#ifndef CROMBIE2_EXCEPTIONS_H
#define CROMBIE2_EXCEPTIONS_H 1

#include <iostream>
#include <exception>

namespace exceptions {

  template<typename T>
    typename T::value_type& at(T& vec, unsigned index, unsigned line) {

    try {
      return vec.at(index);
    }
    catch (const std::exception& e) {
      std::cerr << "Line: " << line << " trying to get index " << index << std::endl;
      throw e;
    }

  }

}

#endif
