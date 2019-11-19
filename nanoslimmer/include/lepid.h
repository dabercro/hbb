#ifndef HBBNANO_LEPID_H
#define HBBNANO_LEPID_H


#include <cmath>

#include "PandaTree/Objects/interface/Electron.h"
#include "PandaTree/Objects/interface/Muon.h"


namespace lepid {


  // From https://gitlab.cern.ch/cms-vhbb/vhbb-nano/blob/master/VHbbProducer.py


  // Muons

  // looseMuons = [x for x in muons if x.pt > 15 and x.tightId >= 1 and x.pfRelIso04_all < 0.15]
  bool loose (const panda::Muon& mu) {
    return mu.pt > 15 and mu.tightId >= 1 and mu.pfRelIso04_all < 0.15;
  }

  // tightMuons = [x for x in muons if x.pt > 25 and x.tightId >= 1 and x.pfRelIso04_all < 0.15]
  bool tight (const panda::Muon& mu) {
    return mu.pt > 25 and mu.tightId >= 1 and mu.pfRelIso04_all < 0.15;
  }

  // wMuons = [x for x in muons if x.pt > 25 and x.tightId >= 1 and x.pfRelIso04_all < 0.15 and abs(x.dxy) < 0.05 and abs(x.dz) < 0.2]
  bool W (const panda::Muon& mu) {
    return mu.pt > 25 and mu.tightId >= 1 and mu.pfRelIso04_all < 0.15 and std::abs(mu.dxy) < 0.05 and std::abs(mu.dz) < 0.2;
  }

  // zMuons = [x for x in muons if x.pt > 20 and x.pfRelIso04_all < 0.25 and abs(x.dxy) < 0.05 and abs(x.dz) < 0.2]
  bool Z (const panda::Muon& mu) {
    return mu.pt > 20 and mu.pfRelIso04_all < 0.25 and std::abs(mu.dxy) < 0.05 and std::abs(mu.dz) < 0.2;
  }


  // Electrons

  // looseElectrons = [x for x in electrons if self.elid(x,"80") and x.pt > 15 and abs(x.eta)<2.4]
  bool loose (const panda::Electron& ele) {
    return ele.mvaFall17V2Iso_WP80 and ele.pt > 15 and std::abs(ele.eta) < 2.4;
  }

  // tightElectrons = [x for x in electrons if self.elid(x,"80") and x.pt > 25 and abs(x.eta)<2.4]
  bool tight (const panda::Electron& ele) {
    return ele.mvaFall17V2Iso_WP80 and ele.pt > 25 and std::abs(ele.eta) < 2.4;
  }

  // wElectrons = [x for x in electrons if self.elid(x,"80") and x.pt > 25 and x.pfRelIso03_all < 0.12]
  bool W (const panda::Electron& ele) {
    return ele.mvaFall17V2Iso_WP80 and ele.pt > 25 and ele.pfRelIso03_all < 0.12;
  }

  // zElectrons = [x for x in electrons if x.pt > 20 and self.elid(x,"90") and x.pfRelIso03_all < 0.15]
  bool Z (const panda::Electron& ele) {
    return ele.pt > 20 and ele.mvaFall17V2Iso_WP90 and ele.pfRelIso03_all < 0.15;
  }

}

#endif
