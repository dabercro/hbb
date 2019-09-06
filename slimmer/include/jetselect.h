#ifndef CROMBIE_JETSELECT_H
#define CROMBIE_JETSELECT_H 1

#include <iostream>
#include <memory>

#include <debugevent.h>
#include <input.h>

#include "crombie/KinematicFunctions.h"

#include "PandaTree/Utils/interface/JECCorrector.h"
#include "PandaTree/Objects/interface/Event.h"

#include "debugevent.h"
#include "leptonselect.h"

namespace jetselect {

  // The corrected jets and MET
  class UpdateResult {
  public:
    UpdateResult (const panda::JetCollection& ak4jets,
                  const panda::RecoMet& pfmet) :
      ak4jets {ak4jets},
      pfmet {pfmet} {}
    const panda::JetCollection& ak4jets;
    const panda::RecoMet& pfmet;
  };

  // Both selects the type of jets we'll be using and loads the correction
  class JetSelector {
  public:
    JetSelector () :
    corr_ptr {input::jec.size()
        ? std::make_unique<panda::JECCorrector>(std::string("data/jec/") + input::jec, input::ispuppi ? "AK4PFPuppi" : "AK4PFchs")
        : nullptr},
      ak4jets {input::ispuppi ? &panda::Event::puppiAK4Jets : &panda::Event::chsAK4Jets}
    {
      if (input::jec.size())
        std::cout << "Loaded JEC: " << input::jec << std::endl;
    }


    void init (panda::Event& usethis) {
      event = &usethis;
    }


    const UpdateResult update_event () {

      if (not event)
        throw;

      if (corr_ptr) {
        corr_ptr->update_event(*event, event->*ak4jets, event->pfMet);

        return UpdateResult(corr_ptr->get_jets(), corr_ptr->get_met());
      }

      return UpdateResult(event->*ak4jets, event->pfMet);

    }


  private:

    std::unique_ptr<panda::JECCorrector> corr_ptr {};
    panda::JetCollection panda::Event::*ak4jets;

    // Not owned, must call JetSelector::init to set reference to event
    panda::Event* event {nullptr};

  };

  bool clean_jet (const panda::Jet& jet,
                  const leptonselect::SelectedLeptons& leptons,
                  double cleaning_dr2 = std::pow(0.4, 2)) {

    // Loop through both muons and electrons

    for (auto& info : leptons.preselected) {

      auto* lepton = info.particle;
      auto reliso = lepton->combIso()/lepton->pt();

      if (reliso < 0.4 and
          deltaR2(jet.eta(), jet.phi(), lepton->eta(), lepton->phi()) < cleaning_dr2) {
        if (debugevent::debug)
          std::cout << "Jet with pt " << jet.pt() << " cleaned" << std::endl;
        return false;
      }
    }

    return true;

  }

}


#endif
