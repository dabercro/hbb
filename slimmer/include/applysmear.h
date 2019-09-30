#ifndef CROMBIE_APPLYSMEAR_H
#define CROMBIE_APPLYSMEAR_H


#include <map>
#include <string>

#include "myrandom.h"

#include "PandaTree/Objects/interface/Event.h"


namespace {

  const std::map<std::string, double> smear_factors {
    {"190904_0", 0.112730656436}, 
    {"190904_0_2", 0.112730656436},
    {"190723_origin", 0.106471840197},
    {"190723_puppi", 0.115164599911}, 
    {"190725_lstm_pf", 0.10287884027}
  };

}

namespace applysmear {

  double smeared_pt(const panda::Jet& jet, double regression_factor, const std::string& regression) {

    auto& gen = jet.matchedGenJet;

    double regressed = jet.pt() * regression_factor;

    double smear = myrandom::gen.Gaus(0, smear_factors.at(regression)) * (gen.isValid() ? gen->pt() : regressed);

    return regressed + smear;

  }

}


#endif
