#ifndef CROMBIE_FEEDPANDA_H
#define CROMBIE_FEEDPANDA_H 1

#include "TTree.h"

#include "PandaTree/Objects/interface/Event.h"

void feedpanda(panda::Event& event, TTree* input) {
  event.setStatus(*input, {"!*"});
  event.setAddress(*input,
    {"ak4GenJets",
     "caloMet",
     "chsAK4Jets",
     "electrons",
     "eventNumber",
     "genParticles",
     "lumiNumber",
     "metFilters",
     "muons",
     "npv",
     "npvTrue",
     "pfCandidates",
     "pfMet",
     "photons",
     "runNumber",
     "secondaryVertices",
     "taus",
     "tracks",
     "triggers",
     "trkMet",
     "vertices",
     "weight"});
}

#endif
