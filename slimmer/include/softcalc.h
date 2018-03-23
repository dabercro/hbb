#include <cmath>
#include "PandaTree/Objects/interface/Event.h"

namespace softcalc {
  // A class for forming an ellipse in eta-phi plane
  // Determines quickly if panda::Particles are inside it

  class Ellipse {
   public:
    // Create ellipse defined by two jets as foci
    Ellipse (const panda::Particle*, const panda::Particle*, double add = 0.4);
    // Check if a particle is inside this ellipse
    inline bool inside (const panda::Particle& part) const {
      return valid and (deltaR(part.eta(), part.phi(), eta1, phi1) + deltaR(part.eta(), part.phi(), eta2, phi2) <= foci_dist);
      /* and (deltaPhi(part.phi(), center_phi) < deltaPhi(part.phi(), outside_phi)); */ // This is what we should have to pick a side
      // Old analysis didn't do this. Uncomment all the /* */ stuff to do this right (or at least better).
    }

   private:

    const bool valid;

    // Locations of foci
    const double eta1;
    const double phi1;
    const double eta2;
    const double phi2;

    // Points must be closer in phi to center phi than outside_phi to be included
    // This may lead to clipping of ellipse when dphi of jets are close to pi,
    // but goal is questionable then anyway
    /* double center_phi; */
    /* double outside_phi; */

    // Sum of distance of outer point from the two foci
    const double foci_dist;

  };
}

softcalc::Ellipse::Ellipse(const panda::Particle* particle1, const panda::Particle* particle2, double add)
: valid{particle2},
  eta1{valid ? particle1->eta() : 0},
  phi1{valid ? make_pm(particle1->phi()) : 0},
  eta2{valid ? particle2->eta() : 0},
  phi2{valid ? make_pm(particle2->phi()) : 0},
  foci_dist{valid ? deltaR(eta1, phi1, eta2, phi2) + 2.0 * add : 0}
{
  /* bool crosses_seam = (deltaPhi(phi1, phi2) < std::abs(phi1 - phi2)); */

  /* auto avg = (phi1 + phi2) / 2.0; */
  /* center_phi = crosses_seam ? make_pm(avg + crombie::pi) : avg; */
  /* outside_phi = make_pm(center_phi + crombie::pi); */
}
