#ifndef CROMBIE_APPLYSMEAR_H
#define CROMBIE_APPLYSMEAR_H


#include <map>
#include <string>

#include "myrandom.h"

#include "PandaTree/Objects/interface/Event.h"


namespace {

  // From 191002.txt

  /* const std::map<std::string, double> smear_factors { */
  /*   {"190904_0", 0.106512398001},  */
  /*   {"190904_0_2", 0.112276606618}, */
  /*   {"190904_0_3", 0.105195937195}, */
  /*   {"190723_origin", 0.104605280943}, */
  /*   {"190723_puppi", 0.106518562593},  */
  /*   {"190723_origin_2", 0.103510789748}, */
  /*   {"190723_puppi_2", 0.105950893703},  */
  /*   {"190725_lstm_pf", 0.109585952519}, */
  /*   {"190924_0", 0.104578635851} */
  /* }; */

  // From 200103.txt

  const std::map<std::string, double> smear_factors {
    {"190904_0", 0.0795009826793},
    {"190904_0_2", 0.0866740728095},
    {"190904_0_3", 0.075062949948},
    {"190723_origin", 0.0765215871761},
    {"190723_puppi", 0.0770008243601},
    {"190723_origin_2", 0.0750582123909},
    {"190723_puppi_2", 0.0758874550776},
    {"190725_lstm_pf", 0.0814002551971},
    {"190924_0", 0.076048292826}
  };

  // From 191029_res.txt

  const std::map<std::string, std::vector<double>> rhobinned_smear_factors {
    {"190904_0", {0.112753527177, 0.0821811284865, 0.0929594127157}}, 
    {"190904_0_2", {0.111179050498, 0.093095090699, 0.0817841162796}},
    {"190904_0_3", {0.109969881816, 0.0861841950997, 0.0800652163228}},
    {"190723_origin", {0.114348265147, 0.0812484551997, 0.0835998105048}},
    {"190723_origin_2", {0.109464934645, 0.0834817605888, 0.0774172770787}},
    {"190725_lstm_pf", {0.107676202028, 0.0890921104981, 0.0846434598145}},
    {"190924_0", {0.113184294829, 0.0864063842915, 0.0830703914045}}
  };

}

namespace applysmear {

  double smeared_pt(const panda::Jet& jet, double regression_factor, const std::string& regression, double rho) {

    double regressed = jet.pt() * regression_factor;

    auto iter = rhobinned_smear_factors.find(regression);

    unsigned index = (rho >= 16) + (rho >= 22);

    const std::vector<double> defaults {0.103989170404, 0.0950507334439, 0.077574797666};

    // Default value if not found
    double smearfactor = (iter == rhobinned_smear_factors.end() ? defaults : iter->second).at(index);

    // Use generator pt, if available
    auto& gen = jet.matchedGenJet;
    double pt = (gen.isValid() ? gen->pt() : regressed);

    double smear = myrandom::gen.Gaus(0, smearfactor) * pt;

    return regressed + smear;

  }

}


#endif
