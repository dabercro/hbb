#ifndef CROMBIE_FEEDPANDA_H
#define CROMBIE_FEEDPANDA_H 1

#include "TTree.h"

#include "PandaTree/Objects/interface/Event.h"

namespace crombie {
  void feedpanda(panda::Event& event, TTree* input) {
    event.setStatus(*input, {"!*"});
    event.setAddress(*input,
      {"ak4GenJets",
       "eventNumber",
       "genParticles",
       "lumiNumber",
       "npv",
       "pfCandidates",
       "pfMet",
       "puppiAK4Jets",
       "rho",
       "runNumber",
       "secondaryVertices",
       "tracks",
       "triggers",
       "vertices",
       "weight"  });
  }
}

#endif
