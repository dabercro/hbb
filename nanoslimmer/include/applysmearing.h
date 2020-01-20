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

  const fit_result convoluted_fit {
    -1.10794e-03,
    2.75232e-03,
    1.27932e-01,
    5.53656e-02,
    -0.000146337193845
  };

  const fit_result scaling_fit {
    -3.20061e-03,
    1.12476e-02,
    2.76458e-01,
    2.37066e-01,
    -0.00252854286916
  };

  const fit_result old_scale {
    0, 0, 0.1, 0.1, 0
  };


  // 200116_1D_bins_allenv.txt
  const fit_result single_bin_scale {
    0, 0, 0.063854, 0.071989, 0
  };
  const fit_result single_bin_smear {
    0, 0, 0.051249, 0.027145, 0
  };

  // 200119/Gaussian_xsec_weight.txt
  const fit_result unbinned_scale {
    0, 0, 0.054713, 0.064761, 0
  };
  const fit_result unbinned_smear {
    0, 0, 0.046527, 0.026636, 0
  };
  
}


namespace applysmearing {

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
    SMEAR,
    SCALE,
    OLD_SCALE,
    SINGLE_SCALE,
    SINGLE_SMEAR,
    UNBINNED_SCALE,
    UNBINNED_SMEAR
  };

  smear_result smeared_pt (double jet_pt, double regression_factor, double rho, double gen_jet_pt = 0, smear_method method = smear_method::SMEAR) {

    double regressed = jet_pt * regression_factor;

    // Use the result that matches the desired smearing
    const fit_result& fit = std::map<smear_method, fit_result> {
      {smear_method::SMEAR, convoluted_fit},
      {smear_method::SCALE, scaling_fit},
      {smear_method::OLD_SCALE, old_scale},
      {smear_method::SINGLE_SCALE, single_bin_scale},
      {smear_method::SINGLE_SMEAR, single_bin_smear},
      {smear_method::UNBINNED_SCALE, unbinned_scale},
      {smear_method::UNBINNED_SMEAR, unbinned_smear}
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
        std::max(0.0, gen_jet_pt + (1.0 + nominal) * dpt),          // nominal smearing
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
