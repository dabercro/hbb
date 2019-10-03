#ifndef CROMBIE_APPLYSMEAR_H
#define CROMBIE_APPLYSMEAR_H


#include <map>
#include <string>

#include "myrandom.h"

#include "PandaTree/Objects/interface/Event.h"


namespace {

  const std::map<std::string, double> smear_factors {
    {"190904_0", 0.106512398001}, 
    {"190904_0_2", 0.112276606618},
    {"190904_0_3", 0.105195937195},
    {"190723_origin", 0.104605280943},
    {"190723_puppi", 0.106518562593}, 
    {"190723_origin_2", 0.103510789748},
    {"190723_puppi_2", 0.105950893703}, 
    {"190725_lstm_pf", 0.109585952519},
    {"190924_0", 0.104578635851}
  };

}

namespace applysmear {

  double smeared_pt(const panda::Jet& jet, double regression_factor, const std::string& regression) {

    double regressed = jet.pt() * regression_factor;

    auto iter = smear_factors.find(regression);

    // Default value if not found
    double smearfactor = (iter == smear_factors.end() ? 0.1 : iter->second);

    // Use generator pt, if available
    auto& gen = jet.matchedGenJet;
    double pt = (gen.isValid() ? gen->pt() : regressed);

    double smear = myrandom::gen.Gaus(0, smearfactor) * pt;

    return regressed + smear;

  }

}


#endif
