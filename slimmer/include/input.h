#ifndef INPUT_H
#define INPUT_H


#include <cstdlib>
#include <string>
#include <sstream>
#include <regex>
#include <iostream>

namespace {

  static auto* in_dir = getenv("input_dir");
  auto is = [] (const char* search) {
    return in_dir and std::string(in_dir).find(search) != std::string::npos;
  };

}

namespace input {

  const char* noskim = getenv("noskim");

  const char* ispuppi = getenv("ispuppi");

  const bool isHT = is("_HT-");

  const bool isW = is("_HToBB_W");
  const bool isWp = isW and is("Wplus");
  const bool isWm = isW and is("Wminus");

  const bool isZ = is("ZH_HToBB_ZTo");
  const bool isZll = isZ and is("ZLL");
  const bool isZnn = isZ and is("ZNuNu");

  const auto version = [] () {
    std::regex expr {"/pandaf/0*(\\d*)/"};
    std::cmatch matches;
    unsigned short output {};
    if (in_dir and std::regex_search(in_dir, matches, expr)) {
      std::stringstream iss;
      iss << matches[1];
      iss >> output;
    }
    std::cout << "Running over Panda version " << output << std::endl;
    return output;
  } ();

  enum btagger {
    cmva,
    deepCSVb
  };

  const btagger tagger = version <= 9 ? btagger::cmva : btagger::deepCSVb;

  long long maxevents = [] () {
    long long output = 0;
    auto* env = getenv("maxevents");
    if (env) {
      std::stringstream iss;
      iss << env;
      iss >> output;
    }
    return output;
  } ();

  const std::string year = [] () {
    std::string indir {in_dir};
    auto pos = indir.find("Run201");
    if (pos != std::string::npos)
      return indir.substr(pos + 3, 4);

    // Otherwise, is MC
    if (version == 9)
      return std::string("2016");
    if (version == 12)
      return std::string("2017");
    return std::string("2018");
  } ();

  const std::string jec = [] () {
    if (version >= 13) {
      std::string data = "Run2018";
      std::string start = "Winter19_Autumn18";

      if (is(data.data())) {

        std::string indir {in_dir};
        auto letter = indir.substr(indir.find(data) + data.size(), 1);
        if (letter == "D" or letter == "E")
          letter = "DE";
        return std::string(start + letter + "_V8_DATA");
      }
      // Otherwise, is MC
      return std::string(start + "_V8_MC");
    }
    return std::string("");
  } ();

}


#endif
