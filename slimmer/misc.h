#include <cmath>
#include "TLorentzVector.h"

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

double foci_shift(double deltaR) {

  return (deltaR + 1 - std::sqrt(std::pow(deltaR, 2) + 2 * deltaR))/2.0;

}
