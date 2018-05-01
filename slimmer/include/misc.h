#ifndef SLIMMER_MISC_H
#define SLIMMER_MISC_H

#include <cmath>
#include <utility>

#include "TLorentzVector.h"

#include "PlotTools/interface/KinematicFunctions.h"

#include "feedpanda.h"
#include "checkrun.h"
#include "btagreaders.h"
#include "debugevent.h"
#include "roccor.h"
#include "corrections.h"
#include "softcalc.h"
#include "lazy.h"
#include "electronid.h"
#include "puid.h"
#include "trkmet.h"
#include "reliso.h"

// Hold information about genjet vectors with neutrinos added

class GenNuVec {
 public:
  GenNuVec(TLorentzVector vec = TLorentzVector{}) : genvec{vec} {}

  TLorentzVector genvec;
  int numnu {0};
  bool overlap {false};

  void add_nu(panda::GenParticle& nu) {
    auto& parent = nu.parent;
    if (parent.isValid() and abs(parent->pdgid) == 24 and parent->m() > 50)  // Don't add neutrinos from massive W to jet
      overlap = true;
    else {
      ++numnu;
      genvec += nu.p4();
    }
  }
};

// Define a structure to hold all additional variables about leptons we want to save

class LepInfo {
 public:
  LepInfo (double reliso = 0, double mini = 0, double corrpt = 0) : reliso{reliso}, minireliso{mini}, corrpt{corrpt} {}

  double reliso;
  double minireliso;
  double corrpt; // Rochester corrected pT (muons) or Smeared pT (electrons)
};


#endif
