#ifndef HBBNANO_APPLYSMEARING_H
#define HBBNANO_APPLYSMEARING_H


#include <cmath>

#include "TRandom3.h"


namespace applysmearing {

  struct smear_result {

    double down;
    double nominal;
    double up;

    operator double () { return nominal; }

  };

  TRandom3 generator {};

  const double slope = -1.10794e-03;
  const double slope_err = 2.75232e-03;
  const double intercept = 1.27932e-01;
  const double intercept_err = 5.53656e-02;
  const double covar = -0.000146337193845;

  double nominal_smear (double rho) {
    return slope * rho + intercept;
  }

  double smear_band (double rho) {
    return std::sqrt(std::pow((rho + covar * intercept) * slope_err, 2) +
                     std::pow((covar * slope * rho + 1) * intercept_err, 2));
  }

  smear_result smeared_pt (double jet_pt, double regression_factor, double rho, double gen_jet_pt = 0) {

    double regressed = jet_pt * regression_factor;

    // Use generator pt, if available, otherwise best guess
    double pt = (gen_jet_pt ? gen_jet_pt : regressed);

    double gaus_sample = generator.Gaus();

    auto nominal = nominal_smear(rho);
    auto band = smear_band(rho);

    smear_result output {
      std::max(0.0, regressed + gaus_sample * std::max(nominal - band, 0.0) * pt), // less smearing
      std::max(0.0, regressed + gaus_sample * std::max(nominal, 0.0) * pt),        // nominal smearing
      std::max(0.0, regressed + gaus_sample * std::max(nominal + band, 0.0) * pt)  // more smearing
    };

    return output;

  }

  smear_result old_smear (double jet_pt, double regression_factor, double gen_jet_pt = 0) {

    double regressed = jet_pt * regression_factor;

    if (gen_jet_pt) {
      double dpt = regressed - gen_jet_pt;

      smear_result output {
        std::max(0.0, gen_jet_pt + 1.0 * dpt), // less smearing
        std::max(0.0, gen_jet_pt + 1.1 * dpt), // nominal smearing
        std::max(0.0, gen_jet_pt + 1.2 * dpt)  // more smearing
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
