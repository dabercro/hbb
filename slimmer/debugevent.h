#ifndef CROMBIE_DEBUG_EVENT
#define CROMBIE_DEBUG_EVENT

#include <cstdlib>
#include <tuple>
#include <string>
#include <sstream>

namespace {
  char* rle = getenv("debugevent");
  std::tuple<unsigned, unsigned, unsigned long> rle_tuple;
}

namespace debug {
  const bool debug = rle;
  void init() {
    if (debug) {
      std::stringstream iss;
      iss << rle;
      iss >> std::get<0>(rle_tuple)
          >> std::get<1>(rle_tuple)
          >> std::get<2>(rle_tuple);
    }
  }
  bool check(unsigned run, unsigned lumi, unsigned long event) {
    return rle_tuple == std::make_tuple(run, lumi, event);
  }
};

#endif
