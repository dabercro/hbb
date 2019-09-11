#ifndef CROMBIE_ROCCOR_H
#define CROMBIE_ROCCOR_H 1


#include "roccor/RoccoR.cc" // https://twiki.cern.ch/twiki/bin/view/CMS/RochcorMuon


namespace {
  RoccoR rochester {"data/roccor/RoccoR2018.txt"};
}

namespace roccor {
  double scale(const panda::Event& event, const panda::Muon& muon) {

    if (event.isData)
      return rochester.kScaleDT(muon.charge, muon.pt(), muon.eta(), muon.phi());

    auto& gen = muon.matchedGen;
    if (gen.isValid())
      return rochester.kSpreadMC(muon.charge, muon.pt(), muon.eta(), muon.phi(), gen->pt());

    return 1.0;

  }
}


#endif
