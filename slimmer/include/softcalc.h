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
      return valid and (KinematicFunctions::deltaR(part.eta(), part.phi(), eta1, phi1) + KinematicFunctions::deltaR(part.eta(), part.phi(), eta2, phi2) <= foci_dist) and
        (KinematicFunctions::deltaPhi(part.phi(), center_phi) < KinematicFunctions::deltaPhi(part.phi(), outside_phi)); // This is what we should have to pick a side
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
    double center_phi;
    double outside_phi;

    // Sum of distance of outer point from the two foci
    const double foci_dist;

  };
}

softcalc::Ellipse::Ellipse(const panda::Particle* particle1, const panda::Particle* particle2, double add)
: valid{particle2},
  eta1{valid ? particle1->eta() : 0},
  phi1{valid ? KinematicFunctions::make_pm(particle1->phi()) : 0},
  eta2{valid ? particle2->eta() : 0},
  phi2{valid ? KinematicFunctions::make_pm(particle2->phi()) : 0},
  foci_dist{valid ? KinematicFunctions::deltaR(eta1, phi1, eta2, phi2) + 2.0 * add : 0}
{
  bool crosses_seam = (KinematicFunctions::deltaPhi(phi1, phi2) < std::abs(phi1 - phi2));

  auto avg = (phi1 + phi2) / 2.0;
  center_phi = crosses_seam ? KinematicFunctions::make_pm(avg + crombie::pi) : avg;
  outside_phi = KinematicFunctions::make_pm(center_phi + crombie::pi);
}

class DylanSoft {
 public:
  DylanSoft (float eta1, float phi1, float eta2, float phi2) {
    double ellipse_alpha;
    double phi1MinusPhi2 = phi1-phi2;
    double eta1MinusEta2 = eta1-eta2;
    double phi1MinusPhi2MPP = TVector2::Phi_mpi_pi(phi1MinusPhi2);
    ellipse_alpha = std::atan2( phi1MinusPhi2, eta1MinusEta2);
    // compute KinematicFunctions::delta R using already computed qty's to save time
    ellipse_a = (std::sqrt(std::pow(eta1MinusEta2,2) 
                           + std::pow(TVector2::Phi_mpi_pi(phi1MinusPhi2),2)) + 1.)/2.; // Major axis 2*a = dR(b,b)+1
    ellipse_b = 1./2.; // Minor axis 2*b = 1
    ellipse_h = (eta1+eta2)/2.;
    ellipse_k = TVector2::Phi_mpi_pi(phi2 + phi1MinusPhi2MPP/2.);
    ellipse_cosA = std::cos(ellipse_alpha);
    ellipse_sinA = std::sin(ellipse_alpha);
  }
  double ellipse_cosA, ellipse_sinA, ellipse_h, ellipse_k, ellipse_a, ellipse_b;

  double ellipse_eq (double eta, double phi) {
    double ellipse_term1 = std::pow(
                               (TVector2::Phi_mpi_pi(eta - ellipse_h)*ellipse_cosA
                                + (phi - ellipse_k)*ellipse_sinA) / ellipse_a,
                               2
                               );
    double ellipse_term2 = std::pow(
                               (TVector2::Phi_mpi_pi(eta - ellipse_h)*ellipse_sinA
                                - (phi - ellipse_k)*ellipse_cosA) / ellipse_b,
                               2
                               );

    return ellipse_term1 + ellipse_term2;
  }
};
