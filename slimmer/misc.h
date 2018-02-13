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

  void count (float value, int& loose_count, int& medium_count, int& tight_count) const {
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
  // Create ellipse defined by two foci (not in the exact same place as the jets)
  // Major axis = DR + 1; Minor axis = 1;
  Ellipse (panda::Particle&, panda::Particle&);
  // Check if a particle is inside this ellipse
  inline bool inside (panda::Particle& part) {
    return (deltaR(part.eta(), part.phi(), eta1, phi1) + deltaR(part.eta(), part.phi(), eta2, phi2) <= foci_dist);
  }

 private:
  // The shift to get the foci from the deltaR between the two b jets
  inline static double foci_shift(double deltaR) {
    return (deltaR + 1 - std::sqrt(std::pow(deltaR, 2) + 2 * deltaR))/2.0;
  }

  double eta1;
  double phi1;
  double eta2;
  double phi2;

  double foci_dist;
};

Ellipse::Ellipse(panda::Particle& particle1, panda::Particle& particle2) {

  auto deta = particle1.eta() - particle2.eta();
  auto dphi = deltaPhi(particle1.phi(), particle2.phi());

  auto dR = std::sqrt(deta * deta + dphi * dphi);

  foci_dist = dR + 1;

  auto shift = foci_shift(dR);

  auto shift_eta = shift * deta / dR;
  auto shift_phi = shift * dphi / dR;

  // Determine which direction to do each shift
  eta1 = particle1.eta() + shift_eta;
  eta2 = particle2.eta() - shift_eta;

  phi1 = make_pm(particle1.phi());
  phi2 = make_pm(particle2.phi());

  // If phi1 > phi2 xor crossing the phi = pi line, phi1 increases, and phi2 decreases
  if ((phi1 > phi2) != (deltaPhi(phi1, phi2) < fabs(phi1 - phi2))) {
    phi1 += shift_phi;
    phi2 -= shift_phi;
  }
  else {
    phi1 -= shift_phi;
    phi2 += shift_phi;
  }

  // Make sure phis are where they are supposed to be
  auto spin = [] (double& phi) {
    if (phi >= pi) {
      phi -= 2 * pi;
    }
    if (phi < -pi) {
      phi += 2 * pi;
    }
  };
  spin(phi1);
  spin(phi2);
}

#endif

