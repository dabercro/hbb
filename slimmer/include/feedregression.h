#ifndef CROMBIE_FEEDPANDA_H
#define CROMBIE_FEEDPANDA_H 1

#include "TTree.h"

#include "PandaTree/Objects/interface/Event.h"

void feedpanda(panda::Event& event, TTree* input) {
  event.setStatus(*input, {"!*"});
  event.setAddress(*input,
    {"ak4GenJets",
     "chsAK4Jets",
     "eventNumber",
     "genParticles",
     "lumiNumber",
     "npv",
     "npvTrue",
     "pfCandidates",
     "pfMet",
     "rho",
     "runNumber",
     "secondaryVertices",
     "tracks",
     "triggers",
     "weight"});
}

#endif
