#ifndef SLIMMER_MISC_H
#define SLIMMER_MISC_H

#include <cmath>
#include <utility>

#include "TLorentzVector.h"

#include "PandaTree/Objects/interface/Event.h"
#include "PlotTools/interface/KinematicFunctions.h"

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

// A quick class for counting B-tags. Can be used for any other object that just compares one value

class BTagCounter {
 public:
  constexpr BTagCounter (float loose, float medium, float tight)
    : loose(loose), medium(medium), tight(tight) {}

  template <typename T>
    void count (float value, T& loose_count, T& medium_count, T& tight_count) const {
    if (value > loose) {
      loose_count++;
      if (value > medium) {
        medium_count++;
        if (value > tight)
          tight_count++;
      }
    }
  }

 private:  
  const float loose;
  const float medium;
  const float tight;
};

// A class for forming an ellipse in eta-phi plane
// Determines quickly if panda::Particles are inside it

class Ellipse {
 public:
  // Create ellipse defined by two jets as foci
  Ellipse (panda::Particle&, panda::Particle&, double add = 0.4);
  // Check if a particle is inside this ellipse
  inline bool inside (panda::Particle& part) {
    return (deltaR(part.eta(), part.phi(), eta1, phi1) + deltaR(part.eta(), part.phi(), eta2, phi2) <= foci_dist);
    /* and (deltaPhi(part.phi(), center_phi) < deltaPhi(part.phi(), outside_phi)); */ // This is what we should have to pick a side
    // Old analysis didn't do this. Uncomment all the /* */ stuff to do this right (or at least better).
  }

 private:

  // Locations of foci
  double eta1;
  double phi1;
  double eta2;
  double phi2;

  // Points must be closer in phi to center phi than outside_phi to be included
  // This may lead to clipping of ellipse when dphi of jets are close to pi,
  // but goal is questionable then anyway
  /* double center_phi; */
  /* double outside_phi; */

  // Sum of distance of outer point from the two foci
  double foci_dist;
};

Ellipse::Ellipse(panda::Particle& particle1, panda::Particle& particle2, double add)
: eta1{particle1.eta()}, phi1{make_pm(particle1.phi())},
  eta2{particle2.eta()}, phi2{make_pm(particle2.phi())},
  foci_dist{deltaR(particle1.eta(), particle1.phi(), particle2.eta(), particle2.phi()) + 2.0 * add}
{
  /* bool crosses_seam = (deltaPhi(phi1, phi2) < std::abs(phi1 - phi2)); */

  /* auto avg = (phi1 + phi2) / 2.0; */
  /* center_phi = crosses_seam ? make_pm(avg + crombie::pi) : avg; */
  /* outside_phi = make_pm(center_phi + crombie::pi); */
}

// Define a structure to hold all additional variables about leptons we want to save

class LepInfo {
 public:
  enum SelectionFlag {
    presel = 0,
    loose = 1,
    medium = 2,
    tight = 3
  };

  LepInfo (SelectionFlag flag = presel, float reliso = 0) : flag{flag}, reliso{reliso} {}

  SelectionFlag flag;
  float reliso;
};

#endif
