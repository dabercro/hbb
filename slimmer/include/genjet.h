#ifndef GENJET_H
#define GENJET_H


#include <cmath>
#include <map>

#include "input.h"

#include "TLorentzVector.h"

#include "crombie/KinematicFunctions.h"

#include "PandaTree/Objects/interface/Event.h"

namespace gennujet {

  // Hold information about genjet vectors with neutrinos added

  class GenNuVec {
  public:
    GenNuVec(TLorentzVector vec = TLorentzVector{}) : genvec{vec} {}

    TLorentzVector genvec;
    int numnu {0};
    bool overlap {false};

    void add_nu(const panda::GenParticle& nu) {
      auto& parent = nu.parent;
      if (parent.isValid() and abs(parent->pdgid) == 24 and parent->m() > 50)  // Don't add neutrinos from massive W to jet
        overlap = true;
      else {
        ++numnu;
        genvec += nu.p4();
      }
    }

  };


  const std::map<const panda::GenJet*, GenNuVec> get_gen_nu_map (const panda::Event& event) {

    std::map<const panda::GenJet*, GenNuVec> output {};

    for (auto& gen_jet : event.ak4GenJets)
      output[&gen_jet] = {gen_jet.p4()};

    for (auto& gen : event.genParticles) {
      auto abspdgid = std::abs(gen.pdgid);

      if ((abspdgid == 12 or abspdgid == 14 or abspdgid == 16) and (input::version < 12 or gen.miniaodPacked)) {
        // Check jets of each collection for closest jet to neutrinos and add to the genvec stored in jetstore's extra
        float cone_size = std::pow(0.4, 2);   // Neutrinos must be within deltaR2 = (0.4)^2
        const panda::GenJet* closest = nullptr;
        for (auto& gen_jet : event.ak4GenJets) {
          auto check = deltaR2(gen_jet.eta(), gen_jet.phi(), gen.eta(), gen.phi());
          // If the neutrino momentum is super high, probably not from this jet, so scale by anti-kt metric
          if (gen.pt() > gen_jet.pt())
            check *= std::pow(gen.pt()/gen_jet.pt(), 2);
          if (check < cone_size) {
            cone_size = check;
            closest = &gen_jet;
          }
        }
        if (closest)
          output.at(closest).add_nu(gen);

      }

    }

    return output;

  }

}


#endif
