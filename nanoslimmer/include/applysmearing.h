#ifndef HBBNANO_APPLYSMEARING_H
#define HBBNANO_APPLYSMEARING_H


#include "TRandom3.h"

namespace {

  // From 191117.txt

  // First index is different rho bins
  // Next index cycles through less smearing, nominal, more smearing
  const std::vector<std::vector<double>> smearings = {
    {0.113394 - 0.022230, 0.113394, 0.113394 + 0.022230},
    {0.086259 - 0.028544, 0.086259, 0.086259 + 0.028544},
    {0.077688 - 0.036912, 0.077688, 0.077688 + 0.036912}
  };

  TRandom3 gen {};

}

namespace applysmearing {

  struct smear_result {

    double down;
    double nominal;
    double up;

    operator double () { return nominal; }

  };

  smear_result smeared_pt(double jet_pt, double regression_factor, double rho, double gen_jet_pt = 0) {

    double regressed = jet_pt * regression_factor;

    unsigned index = (rho >= 16) + (rho >= 22);

    // Use generator pt, if available, otherwise best guess
    double pt = (gen_jet_pt ? gen_jet_pt : regressed);

    double gaus_sample = gen.Gaus();

    smear_result output {
      regressed + gaus_sample * smearings[index][0] * pt, // less smearing
      regressed + gaus_sample * smearings[index][1] * pt, // nominal smearing
      regressed + gaus_sample * smearings[index][2] * pt  // more smearing
    };

    return output;

  }

}


#endif
