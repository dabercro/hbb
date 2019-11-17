#ifndef HBBNANO_CLEANING_H
#define HBBNANO_CLEANING_H

#include "PandaTree/Objects/interface/Event.h"


namespace cleaning {

  // Returns true if the jet is cleaned
  bool cleanjet (const panda::Jet& jet, const panda::Event& event) {

    if (jet.cleanmask)
      return true;

    auto check_index = [&event] (int index, const auto& coll) {
      if (index > -1 and lepid::presel(coll.at(index)))
        return false;
      return true;
    };

    return
      check_index(jet.muonIdx1, event.Muon) and
      check_index(jet.muonIdx2, event.Muon) and
      check_index(jet.electronIdx1, event.Electron) and
      check_index(jet.electronIdx2, event.Electron);

  }

}


#endif
