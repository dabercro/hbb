#ifndef HBBNANO_JETID_H
#define HBBNANO_JETID_H


#include <cmath>
#include <utility>

#include "cleaning.h"

#include "PandaTree/Objects/interface/Event.h"


namespace jetid {

  bool to_smear (const panda::Jet& jet) {
    return jet.pt * jet.bRegCorr > 20 and jet.jetId > 0 and jet.puId > 0;
  }

  bool higgs (const panda::Jet& jet) {
    return jet.pt * jet.bRegCorr > 35 and jet.jetId > 0 and jet.puId > 0;
  }


  std::pair<const panda::Jet*, const panda::Jet*> higgsjets (const panda::Event& event) {

    std::pair<const panda::Jet*, const panda::Jet*> output {};

    for (auto& jet : event.Jet) {
      if (higgs(jet)) {

        if (not output.first or jet.btagDeepB > output.first->btagDeepB) {
          output.second = output.first;
          output.first = &jet;
        }

        else if (not output.second or jet.btagDeepB > output.second->btagDeepB)
          output.second = &jet;

      }
    }

    return output;

  }

  bool medium_b (const panda::Jet& jet) {
    return jet.btagDeepB > 0.4184;
  }

}


#endif
