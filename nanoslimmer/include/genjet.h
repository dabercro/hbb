#ifndef HBBNANO_GENJET_H
#define HBBNANO_GENJET_H


#include "PandaTree/Objects/interface/Event.h"

#include "crombie/KinematicFunctions.h"


namespace genjet {

  const panda::GenJet* matched (const panda::Jet& jet, const panda::Event& event) {

    const panda::GenJet* output = nullptr;

    double distance = 0.6;

    for (auto& genjet : event.GenJet) {
      double check = deltaR(jet.eta, jet.phi, genjet.eta, genjet.phi);

      if (check < distance and std::abs(jet.pt - genjet.pt)/genjet.pt < 0.5) {
        distance = check;
        output = &genjet;
      }

    }

    return output;

  }

}


#endif
