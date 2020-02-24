#ifndef HBBNANO_APPLYSMEARING_H
#define HBBNANO_APPLYSMEARING_H


#include <cmath>

#include "TRandom3.h"


namespace {

  struct fit_result {
    const double slope;
    const double slope_err;
    const double intercept;
    const double intercept_err;
    const double covar;
  };

  struct all_smears {
    const fit_result binned_scale;
    const fit_result binned_smear;
    const fit_result single_bin_scale;
    const fit_result single_bin_smear;
    const fit_result unbinned_scale;
    const fit_result unbinned_smear;
  };

  // This is the same for everything
  const fit_result old_scale {
    0, 0, 0.1, 0.1, 0
  };

  const std::map<std::string, all_smears> smear_versions {
    {"2018_v5", {
        // 200128_2018v5.txt
        { // Scaling
          -1.09307e-03,
          7.03310e-03,
          3.81715e-02,
          1.55460e-01,
          -0.00103353662013
        },
        { // Smearing
          -2.11521e-03,
          4.08882e-03,
          1.05890e-01,
          8.23771e-02,
          -0.000330959563271
        },
        // 200128_2018v5_single.txt
        { // Scaling
          0, 0, 0.035261, 0.065657, 0
        },
        { // Smearing
          0, 0, 0.037395, 0.033565, 0
        },
        // 200119/Gaussian_xsec_weight.txt
        { // Scaling
          0, 0, 0.054713, 0.064761, 0
        },
        { // Smearing
          0, 0, 0.046527, 0.026636, 0
        }
      }
    },
    {"2018_v6", {
        // 200128_2018v6.txt
        { // Scaling
          -6.69240e-04,
          7.14246e-03,
          5.99045e-02,
          1.57972e-01,
          -0.00106699718945
        },
        { // Smearing
          -6.85226e-03,
          1.03989e-02,
          2.24790e-01,
          2.06139e-01,
          -0.00213620716684
        },
        // 200128_2018v6_single.txt
        { // Scaling
          0, 0, 0.059221, 0.066695, 0
        },
        { // Smearing
          0, 0, 0.048799, 0.025847, 0
        },
        // 200119/Gaussian_xsec_weight.txt
        { // Scaling
          0, 0, 0.054713, 0.064761, 0
        },
        { // Smearing
          0, 0, 0.046527, 0.026636, 0
        }
      }
    },
    {"2018_custom", {
        /* // 200221_2rho_custom_2018.txt */
        /* { // Scaling */
        /*  -2.00099e-03, */
        /*   8.00026e-03, */
        /*   1.14461e-01, */
        /*   1.80780e-01, */
        /*  -0.00139099116147 */
        /* }, */
        /* { // Smearing */
        /*  -3.74186e-04, */
        /*   2.60363e-03, */
        /*   5.85725e-02, */
        /*   5.44750e-02, */
        /*  -0.000135294711453 */
        /* }, */
        // 200223_3rho_custom_2018.txt
        { // Scaling
         -5.24248e-03,
          5.96983e-03,
          1.28543e-01,
          1.38311e-01,
         -0.000787825015403
        },
        { // Smearing
         -1.17189e-02,
          3.40819e-03,
          2.99672e-01,
          7.54276e-02,
         -0.0002523609463
        },
        // 200222_smear_2rho_custom_2018.txt
        { // Scaling
          0, 0, 0.062248, 0.060753, 0
        },
        { // Smearing
          0, 0, 0.047811, 0.021748, 0
        },
        // 200221_custom/Gaussian_xsec_weight.txt
        { // Scaling
          0, 0, 0.090448, 0.011416, 0
        },
        { // Smearing
          0, 0, 0.071067, 0.004363, 0
        }
      }
    }
  };

  const all_smears* loaded = &smear_versions.at("2018_v5");

}


namespace applysmearing {

  void load (const std::string& year) {
    loaded = &smear_versions.at(year);
  }

  struct smear_result {

    double down;
    double nominal;
    double up;

    operator double () { return nominal; }

  };

  TRandom3 generator {};

  double nominal_smear (double rho, const fit_result& fit) {
    return fit.slope * rho + fit.intercept;
  }

  double smear_band (double rho, const fit_result& fit) {
    return std::sqrt(std::pow((rho + fit.covar * fit.intercept) * fit.slope_err, 2) +
                     std::pow((fit.covar * fit.slope * rho + 1) * fit.intercept_err, 2));
  }

  enum class smear_method {
    OLD_SCALE,
    SCALE,
    SMEAR,
    SINGLE_SCALE,
    SINGLE_SMEAR,
    UNBINNED_SCALE,
    UNBINNED_SMEAR
  };

  smear_result smeared_pt (double jet_pt, double regression_factor, double rho, double gen_jet_pt = 0, smear_method method = smear_method::SMEAR) {

    double regressed = jet_pt * regression_factor;

    // Use the result that matches the desired smearing
    const fit_result& fit = std::map<smear_method, fit_result> {
      {smear_method::OLD_SCALE, old_scale},
      {smear_method::SCALE, loaded->binned_scale},
      {smear_method::SMEAR, loaded->binned_smear},
      {smear_method::SINGLE_SCALE, loaded->single_bin_scale},
      {smear_method::SINGLE_SMEAR, loaded->single_bin_smear},
      {smear_method::UNBINNED_SCALE, loaded->unbinned_scale},
      {smear_method::UNBINNED_SMEAR, loaded->unbinned_smear}
    }.at(method);

    auto nominal = nominal_smear(rho, fit);
    auto band = smear_band(rho, fit);

    if (method == smear_method::SMEAR or method == smear_method::SINGLE_SMEAR or method == smear_method::UNBINNED_SMEAR) {

      // Use generator pt, if available, otherwise best guess
      double pt = (gen_jet_pt ? gen_jet_pt : regressed);

      double gaus_sample = generator.Gaus();

      smear_result output {
        std::max(0.0, regressed + gaus_sample * std::max(nominal - band, 0.0) * pt), // less smearing
        std::max(0.0, regressed + gaus_sample * std::max(nominal, 0.0) * pt),        // nominal smearing
        std::max(0.0, regressed + gaus_sample * std::max(nominal + band, 0.0) * pt)  // more smearing
      };

      return output;

    }

    else if (gen_jet_pt) {

      double dpt = regressed - gen_jet_pt;

      smear_result output {
        std::max(0.0, gen_jet_pt + (1.0 + nominal - band) * dpt), // less smearing
        std::max(0.0, gen_jet_pt + (1.0 + nominal) * dpt),        // nominal smearing
        std::max(0.0, gen_jet_pt + (1.0 + nominal + band) * dpt)  // more smearing
      };

      return output;

    }

    smear_result output {
      regressed, regressed, regressed
    };

    return output;

  }

}


#endif
