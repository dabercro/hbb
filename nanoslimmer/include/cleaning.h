#ifndef HBBNANO_CLEANING_H
#define HBBNANO_CLEANING_H

#include <cmath>

#include "lepid.h"

#include "TVector2.h"
#include "PandaTree/Objects/interface/Event.h"


namespace cleaning {

  // Returns true if the jet is cleaned
  bool lepfilter (const panda::Jet& jet, const panda::Event& event) {

    auto overlap = [&jet, &event] (const auto& leps) {
      for (auto& lep : leps) {
        if (lep.jetIdx >= 0 and &jet == &event.Jet.at(lep.jetIdx)) {
          if (lepid::Z(lep) or lepid::W(lep))
            return true;
        }
      }
      return false;
    };

    // Don't overlap with muons or electrons
    return not overlap(event.Muon) and not overlap(event.Electron);

  }

  template<typename F, typename S> bool outside (F first, S second, double delta_r) {

    return ((std::pow(first.eta - second.eta, 2) + std::pow(TVector2::Phi_mpi_pi(first.phi - second.phi), 2)) > std::pow(delta_r, 2));

  }

}


#endif
