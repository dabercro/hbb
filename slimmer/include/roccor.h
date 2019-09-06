/* #include "myrandom.h" */
/* #include "roccor/RoccoR.cc" // https://twiki.cern.ch/twiki/bin/view/CMS/RochcorMuon */

/* namespace { */
/*   RoccoR rochester {"data/rcdata.2016.v3"}; */
/* } */

namespace roccor {
  double scale(const panda::Event& event, const panda::Muon& muon) {
    return 1.0;
    /* if (event.isData) */
    /*   return rochester.kScaleDT(muon.charge, muon.pt(), muon.eta(), muon.phi()); */

    /* // Let's generate two number for every MC muon, no matter what */
    /* // If syncing, use the panda::Event to access the generator */
    /* auto roccor_rand1 = myrandom::gen.Rndm(); */
    /* auto roccor_rand2 = myrandom::gen.Rndm(); */

    /* auto& gen = muon.matchedGen; */
    /* if (gen.isValid()) */
    /*   return rochester.kScaleFromGenMC(muon.charge, muon.pt(), muon.eta(), muon.phi(), */
    /*                                    muon.trkLayersWithMmt, gen->pt(), roccor_rand1); */

    /* return rochester.kScaleAndSmearMC(muon.charge, muon.pt(), muon.eta(), muon.phi(), */
    /*                                   muon.trkLayersWithMmt, roccor_rand1, roccor_rand2); */
  }
}
