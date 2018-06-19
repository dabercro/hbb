namespace {

  static auto* in_dir = getenv("input_dir");
  auto is = [] (const char* search) {
    return in_dir and std::string(in_dir).find(search) != std::string::npos;
  };

}

namespace input {

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

}
