#include "TRandom3.h"
#include "RoccoR.cc" // https://twiki.cern.ch/twiki/bin/view/CMS/RochcorMuon

namespace {
  RoccoR rochester {"data/rcdata.2016.v3"};
  unsigned long lastevent = 0;
  TRandom3 roccor_rand;
}

namespace roccor {
  double scale(panda::Event& event, panda::Muon& muon) {
    if (event.isData)
      return 1.0;
      /* return rochester.kScaleDT(muon.charge, muon.pt(), muon.eta(), muon.phi()); */

    // If new event, set the seed
    if (event.eventNumber != lastevent)
      roccor_rand.SetSeed(lastevent = event.eventNumber);

    // Let's generate two number for every MC muon, no matter what
    // (Every muon *MUST* come through this function)
    auto roccor_rand1 = roccor_rand.Rndm();
    auto roccor_rand2 = roccor_rand.Rndm();

    auto& gen = muon.matchedGen;
    if (gen.isValid())
      return rochester.kScaleFromGenMC(muon.charge, muon.pt(), muon.eta(), muon.phi(),
                                       muon.trkLayersWithMmt, gen->pt(), roccor_rand1);

    return rochester.kScaleAndSmearMC(muon.charge, muon.pt(), muon.eta(), muon.phi(),
                                      muon.trkLayersWithMmt, roccor_rand1, roccor_rand2);
  }
}
