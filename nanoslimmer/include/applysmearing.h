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
        // 200303_rho_200303_nbjets.txt
        { // Scaling
         -3.74814e-03,
          8.91546e-03,
          1.17201e-01,
          1.98126e-01,
         -0.00169676561635
        },// 200221_2rho_custom_2018.txt
        { // Smearing
         -3.74186e-04,
          2.60363e-03,
          5.85725e-02,
          5.44750e-02,
         -0.000135294711453
        },
        /* // 200223_3rho_custom_2018.txt */
        /* { // Scaling */
        /*  -5.24248e-03, */
        /*   5.96983e-03, */
        /*   1.28543e-01, */
        /*   1.38311e-01, */
        /*  -0.000787825015403 */
        /* }, */
        /* { // Smearing */
        /*  -1.17189e-02, */
        /*   3.40819e-03, */
        /*   2.99672e-01, */
        /*   7.54276e-02, */
        /*  -0.0002523609463 */
        /* }, */
        // 200303_smear_200303_nbjets.txt
        { // Scaling
          0, 0, 0.031547, 0.065706, 0
        },// 200222_smear_2rho_custom_2018.txt
        { // Smearing
          0, 0, 0.047811, 0.021748, 0
        },
        // 200303_cb/sum*.txt
        { // Scaling
          0, 0, 0.014086, std::sqrt(std::pow(0.052549, 2) + std::pow((0.033231 - 0.000146)/2, 2) + std::pow((0.021255 + 0.005589)/2, 2)), 0
        },// 200223_custom/sum_xsec_weight.txt
        { // Smearing
          0, 0, 0.053905, std::sqrt(std::pow(0.009251, 2) + std::pow((0.061810 - 0.030553)/2, 2)), 0
        }
      }
    }
  };

  struct unbinned_3d {
    double mc_intercept;
    double mc_intercept_err;
    double mc_slope;
    double mc_slope_err;
    double mc_corr;
    double data_intercept;
    double data_intercept_err;
    double data_slope;
    double data_slope_err;
    double data_corr;
  };

  // nominal_simple.txt with data errors for MC
  unbinned_3d fit_3d {
    1.33144e-01,
    1.98080e-02,
   -2.44536e-03,
    1.86825e-04,
   -0.577,
    1.70931e-01,
    1.98080e-02,
   -3.83729e-03,
    1.86825e-04,
   -0.973
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

  double numerator (double rho, const unbinned_3d& fit) {
    return fit.data_intercept + fit.data_slope * rho;
  }

  double denominator (double rho, const unbinned_3d& fit) {
    return fit.mc_intercept + fit.mc_slope * rho;
  }

  double nominal_smear (double rho, const unbinned_3d& fit) {
    return numerator(rho, fit)/denominator(rho, fit) - 1.0;
  }

  double smear_band (double rho, const fit_result& fit) {
    return std::sqrt(std::pow((rho + fit.covar * fit.intercept) * fit.slope_err, 2) +
                     std::pow((fit.covar * fit.slope * rho + 1) * fit.intercept_err, 2));
  }

  double smear_band (double rho_in, const unbinned_3d& fit) {

    auto rho = std::min(rho_in, 45.0d);

    auto num = numerator(rho, fit);
    auto den = denominator(rho, fit);

    return std::sqrt(std::pow(fit.data_intercept_err * (1 + fit.data_corr * fit.data_slope * rho)/den, 2) +
                     std::pow(fit.data_slope_err * (rho + fit.data_corr * fit.data_intercept)/den, 2) +
                     std::pow(fit.mc_intercept_err * (1 + fit.mc_corr * fit.mc_slope * rho) * num/std::pow(den, 2), 2) +
                     std::pow(fit.mc_slope_err * (rho + fit.mc_corr * fit.mc_intercept) * num/std::pow(den, 2), 2)
                     );
  }


  enum class smear_method {
    OLD_SCALE,
    SCALE,
    SMEAR,
    SINGLE_SCALE,
    SINGLE_SMEAR,
    UNBINNED_SCALE,
    UNBINNED_SMEAR,
    UNBINNED_3D
  };

  smear_result smeared_pt (double jet_pt, double regression_factor, double rho, double gen_jet_pt = 0, smear_method method = smear_method::SMEAR) {

    double regressed = jet_pt * regression_factor;

    // Use the result that matches the desired smearing
    const std::map<smear_method, fit_result> fits {
      {smear_method::OLD_SCALE, old_scale},
      {smear_method::SCALE, loaded->binned_scale},
      {smear_method::SMEAR, loaded->binned_smear},
      {smear_method::SINGLE_SCALE, loaded->single_bin_scale},
      {smear_method::SINGLE_SMEAR, loaded->single_bin_smear},
      {smear_method::UNBINNED_SCALE, loaded->unbinned_scale},
      {smear_method::UNBINNED_SMEAR, loaded->unbinned_smear}
    };

    auto nominal = method != smear_method::UNBINNED_3D
      ? nominal_smear(rho, fits.at(method))
      : nominal_smear(rho, fit_3d);

    auto band = method != smear_method::UNBINNED_3D
      ? smear_band(rho, fits.at(method))
      : smear_band(rho, fit_3d);

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
        std::max(0.0, gen_jet_pt + (1.0 + std::max(nominal - band, 0.0)) * dpt), // less smearing
        std::max(0.0, gen_jet_pt + (1.0 + std::max(nominal, 0.0)) * dpt),        // nominal smearing
        std::max(0.0, gen_jet_pt + (1.0 + std::max(nominal + band, 0.0)) * dpt)  // more smearing
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
