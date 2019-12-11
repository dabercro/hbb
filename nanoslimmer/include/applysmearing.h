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
    OLD_SCALE
  };

  smear_result smeared_pt (double jet_pt, double regression_factor, double rho, double gen_jet_pt = 0, smear_method method = smear_method::SMEAR) {

    double regressed = jet_pt * regression_factor;

    // Use the result that matches the desired smearing
    const fit_result& fit = method == smear_method::SMEAR ? convoluted_fit : 
      (method == smear_method::SCALE ? scaling_fit : old_scale);

    auto nominal = nominal_smear(rho, fit);
    auto band = smear_band(rho, fit);

    if (method == smear_method::SMEAR) {

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

    else if ((method == smear_method::SCALE or method == smear_method::OLD_SCALE)
             and gen_jet_pt) {

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
