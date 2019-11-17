#ifndef HBBNANO_LEPID_H
#define HBBNANO_LEPID_H


#include <cmath>

#include "PandaTree/Objects/interface/Electron.h"
#include "PandaTree/Objects/interface/Muon.h"


namespace lepid {


  // Muons

  bool presel (const panda::Muon& mu) {
    return mu.pt > 5.0 and std::abs(mu.eta) < 2.4 and
      mu.dxy < 0.5 and mu.dz < 1.0 and
      mu.miniPFRelIso_all < 0.4;
  }


  bool loose (const panda::Muon& mu) {
    return mu.isPFcand and (mu.isGlobal or mu.isTracker);
  }


  bool tight (const panda::Muon& mu) {
    return mu.tightId and mu.miniPFRelIso_all < 0.15 and
      mu.isGlobal and mu.nStations > 0 and mu.nTrackerLayers > 5 and
      mu.dxy < 0.2 and mu.dz < 0.5;
  }

  // Electrons

  bool presel (const panda::Electron& ele) {
    return ele.pt > 7.0 and std::abs(ele.eta) < 2.4 and
      ele.dxy < 0.05 and ele.dz < 0.20 and ele.miniPFRelIso_all < 0.4;
  }

  bool loose (const panda::Electron& ele) {
    return ele.mvaFall17V2Iso_WP90;
  }

  bool tight (const panda::Electron& ele) {
    return ele.mvaFall17V2Iso_WP80 and
      ele.pt > 15.0 and ele.hoe < 0.09 and ele.pfRelIso03_chg < 0.18;
  }

}

#endif
