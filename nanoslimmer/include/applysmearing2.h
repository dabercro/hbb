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
        {1.013, 0.014,
         0.029, 0.047}
    },
    // outputs/200406_2017_fmt.txt
    {"2017",
        {1.017, 0.021,
         0.058, 0.066}
    },
    // outputs/200506_2018.txt
    {"2018",
        {0.985, 0.019,
         0.080, 0.073}
    },
    // outputs/200331_2018_custom_fmt.txt
    {"2018_custom",
        {0.984, 0.019,
         0.043, 0.072}
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

  smear_result smeared_pt (double jet_pt, double regression_factor, double gen_jet_pt = 0, bool is_data = false, bool do_scale = false) {

    double regressed = std::max(0.0, jet_pt * regression_factor);

    if (is_data) {
      smear_result output {
        regressed, regressed, regressed
      };

      return output;
    }

    double no_smear = regressed * loaded->scale;    // Just scale MC

    if (gen_jet_pt) {

      double gen_diff = regressed - gen_jet_pt;

      double nominal = gen_jet_pt + gen_diff * (1.0 + loaded->smear);
      double band = gen_diff * loaded->smear_err;

      if (do_scale) {

        band = std::sqrt(std::pow(band * loaded->scale, 2) +
                         std::pow(nominal * loaded->scale_err, 2));
        nominal *= loaded->scale;

      }

      nominal = std::max(0.0, nominal);

      double down, up;

      if (regressed > gen_jet_pt) {
        down = std::max(no_smear, nominal - band);
        up = nominal + band;
      }
      else {
        down = std::min(no_smear, nominal + band);
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
