#ifndef CROMBIE_ROCCOR_H
#define CROMBIE_ROCCOR_H 1


#include <string>

#include "roccor/RoccoR.cc" // https://twiki.cern.ch/twiki/bin/view/CMS/RochcorMuon

#include "input.h"
#include "myrandom.h"


namespace {
  RoccoR rochester {std::string("data/roccor/RoccoR") + input::year + ".txt"};
}

namespace roccor {
  double scale(const panda::Event& event, const panda::Muon& muon) {

    if (event.isData)
      return rochester.kScaleDT(muon.charge, muon.pt(), muon.eta(), muon.phi());

    auto& gen = muon.matchedGen;
    if (gen.isValid())
      return rochester.kSpreadMC(muon.charge, muon.pt(), muon.eta(), muon.phi(), gen->pt());

    return rochester.kSmearMC(muon.charge, muon.pt(), muon.eta(), muon.phi(),
                              muon.trkLayersWithMmt, myrandom::gen.Rndm());

  }
}


#endif
