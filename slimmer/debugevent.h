#ifndef CROMBIE_DEBUG_EVENT
#define CROMBIE_DEBUG_EVENT

#include <tuple>
#include <string>
#include <sstream>

namespace {
  const std::string rle_string = "273502 884 1073474132";
  std::tuple<unsigned, unsigned, unsigned long> rle_tuple;
}

namespace debug {
  const bool debug = rle_string.size();
  void init() {
    if (debug){
      std::stringstream iss (rle_string);
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
