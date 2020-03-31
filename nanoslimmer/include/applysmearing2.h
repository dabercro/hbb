#ifndef HBBNANO_APPLYSMEARING2_H
#define HBBNANO_APPLYSMEARING2_H

#include <map>
#include <string>

namespace {

  struct smear_params {
    const double scale;
    const double scale_err;
    const double smear;
    const double smear_err;
  };

  const std::map<std::string, smear_params> smear_versions {
    // outputs/200331_2017.txt
    {"2017",
        {1.0029846959, 0.0212893588055,
         0.030684, 0.052497}
    },
    // outputs/200331_2018_custom.txt
    {"2018_custom",
        {0.98667384694, 0.0197153848807,
         0.038481, 0.053924}
    }
  };

  const smear_params* loaded = &smear_versions.at("2018_custom");

}


namespace applysmearing2 {

  void load (const std::string& year) {
    if (smear_versions.find(year) != smear_versions.end())
      loaded = &smear_versions.at(year);
  }

  struct smear_result {

    double down;
    double nominal;
    double up;

    operator double () { return nominal; }

  };

  smear_result smeared_pt (double jet_pt, double regression_factor, double gen_jet_pt = 0, bool is_data = false) {

    double regressed = std::max(0.0, jet_pt * regression_factor);

    if (is_data) {
      smear_result output {
        regressed, regressed, regressed
      };

      return output;
    }



  }

}


#endif
