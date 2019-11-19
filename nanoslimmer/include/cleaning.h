#ifndef HBBNANO_CLEANING_H
#define HBBNANO_CLEANING_H


#include "lepid.h"

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

}


#endif
