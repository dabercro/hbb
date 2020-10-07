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
    // outputs/201007_2016.txt
    {"2016",
        {0.953, 0.028,
         0.14, 0.10}
    },
    // outputs/201007_2016_v4.txt
    {"2016_v4",
        {0.951, 0.029,
         0.020, 0.099}
    },
    // 201003_2017.txt
    {"2017",
        {0.987, 0.028,
         0.006, 0.086}
    },
    // 201004_2017_v4.txt
    {"2017_v4",
        {0.999, 0.028,
         0.0, 0.086}
    },
    // 201002_2018.txt
    {"2018",
        {0.992, 0.021,
         0.186, 0.069}
    },
    // 201002_2018_v6.txt
    {"2018_v6",
        {1.008, 0.021,
         0.212, 0.074}
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
