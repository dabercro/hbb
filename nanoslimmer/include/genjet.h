#ifndef HBBNANO_GENJET_H
#define HBBNANO_GENJET_H


#include <cmath>

#include "PandaTree/Objects/interface/Event.h"

#include "crombie/KinematicFunctions.h"


namespace genjet {


  const panda::GenJetCollection genjets_withnu (const panda::Event& event) {

    auto output = event.GenJet;

    auto vec = [] (const auto& particle) {
      TLorentzVector out {};
      out.SetPtEtaPhiM(particle.pt, particle.eta, particle.phi, particle.mass);
      return out;
    };

    for (auto& genpart : event.GenPart) {
      if (genpart.status == 1 and
          (std::abs(genpart.pdgId) == 12 or std::abs(genpart.pdgId) == 14 or std::abs(genpart.pdgId) == 16)
          ) {
        for (auto& outjet : output) {
          if(deltaR2(outjet.eta, outjet.phi, genpart.eta, genpart.phi) < std::pow(0.4, 2)) {

            auto newjet = vec(genpart) + vec(outjet);

            outjet.pt = newjet.Pt();
            outjet.eta = newjet.Eta();
            outjet.phi = newjet.Phi();
            outjet.mass = newjet.M();

          }
        }
      }
    }

    return output;

  }


  const panda::GenJet* matched (const panda::Jet& jet, const panda::GenJetCollection& genjets) {

    const panda::GenJet* output = nullptr;

    double distance = 0.6;

    for (auto& genjet : genjets) {
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
