#ifndef CROMBIE_DEBUG_EVENT
#define CROMBIE_DEBUG_EVENT

#include <cstdlib>
#include <tuple>
#include <sstream>

namespace {
  char* rle = getenv("debugevent");
  const auto rle_tuple = [] () {
    std::tuple<unsigned, unsigned, unsigned long> output;  
    if (rle) {
      std::stringstream iss;
      iss << rle;
      iss >> std::get<0>(output)
          >> std::get<1>(output)
          >> std::get<2>(output);
    }
    return output;
  } ();
}

namespace debugevent {
  const bool debugevent = rle;
  const bool debug = debugevent or getenv("debug");
  bool check(unsigned run, unsigned lumi, unsigned long event) {
    return rle_tuple == std::make_tuple(run, lumi, event);
  }
};

#endif
