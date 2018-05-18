namespace {

  const std::string input = std::string("Summer16_23Sep2016") + [] () {
    std::regex expr {"Run201(\\d)(.)"};
    std::cmatch matches;
    auto in_dir = getenv("input_dir");
    if (in_dir and std::regex_search(in_dir, matches, expr)) {
      return [&matches] () {
        // year = matches[1];
        // era = matches[2];
        std::string check {matches[2]};
        switch(check[0]) {
        case('B'):
        case('C'):
        case('D'):
          return std::string("BCD");
        case('E'):
        case('F'):
          return std::string("EF");
        }
        return check; // G and H have their own individual JEC files
      } () + std::string("V4_DATA");
    }
    else  // MC
      return std::string("V4_MC");
    } ();

  /* JetCorrectionUncertainty narrow_unc {}; */

}

namespace splitjec {

  std::map<std::string, float> get_unc() {
    std::cout << input << std::endl;
  }

}
