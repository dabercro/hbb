#ifndef HBBNANO_JETID_H
#define HBBNANO_JETID_H


#include <cmath>
#include <utility>

#include "cleaning.h"


namespace jetid {

  // From https://gitlab.cern.ch/cms-vhbb/vhbb-nano/blob/master/VHbbProducer.py

  // jetsForHiggs = [x for x in jets if x.lepFilter and x.puId>0 and x.jetId>0 and self.pt(x,self.isMC)>20 and abs(x.eta)<2.5]

  bool higgs (const panda::Jet& jet, const panda::Event& event) {
    /* return jet.puId > 0 and jet.jetId > 0 and */
    /*   jet.pt * jet.bRegCorr > 20 and */
    /*   std::abs(jet.eta) < 2.5 and */
    /*   cleaning::lepfilter(jet, event); */
    return jet.pt > 15 and jet.jetId;
  }


  std::pair<const panda::Jet*, const panda::Jet*> higgsjets (const panda::Event& event) {

    std::pair<const panda::Jet*, const panda::Jet*> output {};

    for (auto& jet : event.Jet) {
      if (higgs(jet, event)) {

        if (not output.first or jet.btagDeepB > output.first->btagDeepB) {
          output.first = &jet;
          output.second = output.first;
        }

        else if (not output.second or jet.btagDeepB > output.second->btagDeepB)
          output.second = &jet;

      }
    }

    return output;

  }

}


#endif
