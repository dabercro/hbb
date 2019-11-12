#ifndef HBBNANO_CLEANING_H
#define HBBNANO_CLEANING_H

#include "PandaTree/Objects/interface/Event.h"


namespace cleaning {

  // Returns true if the jet is cleaned
  bool cleanjet (const panda::Jet& jet, const panda::Event& event) {

    if (jet.cleanmask)
      return true;

    auto check_index = [&event] (int index) {
      if (index > -1 and event.Muon.at(index).miniPFRelIso_all < 0.4)
        return false;
      return true;
    };

    return check_index(jet.muonIdx1) and check_index(jet.muonIdx2);

  }

}


#endif
