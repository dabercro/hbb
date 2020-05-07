#ifndef HBBNANO_APPLYSMEARING2_H
#define HBBNANO_APPLYSMEARING2_H

#include <cmath>
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
    // outputs/200401_2016.txt  apparently doesn't need anything
    {"2016",
        {1.0, 0.0,
         0.0, 0.0}
    },
    // outputs/200406_2017_fmt.txt
    {"2017",
        {1.003, 0.021,
         0.031, 0.053}
    },
    // outputs/200331_2018_custom_fmt.txt
    {"2018_custom",
        {0.987, 0.020,
         0.038, 0.054}
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

    double no_smear = regressed * loaded->scale;    // Value if we scale MC

    if (gen_jet_pt) {

      double gen_diff = regressed - gen_jet_pt;
      double down = std::max(0.0, gen_jet_pt + gen_diff * (1.0 + loaded->smear));
      double band = std::sqrt(std::pow(regressed * (1.0 + loaded->smear) * loaded->scale_err, 2) +  // Band on the smear/scaled value
                              std::pow(gen_diff * loaded->smear_err, 2));                           // Actual scaling done is always same

      double nominal, up;

      if (regressed > gen_jet_pt) {
        nominal = down + band;
        up = nominal + band;
      }
      else {
        nominal = down - band;
        up = nominal - band;
      }

      smear_result output {
        down, nominal, up
      };
      return output;

    }

    else {
      smear_result output {
        no_smear, no_smear, no_smear
      };

      return output;
    }

  }

}


#endif
