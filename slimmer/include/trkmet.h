#ifndef CROMBIE_TRKMET_H
#define CROMBIE_TRKMET_H 1

#include <cmath>
#include "TMath.h"
#include "TVector2.h"

namespace {
  // Use this to directly set values you don't build TLorentzVector and TVector3 in between
  TVector2 convert_pfcand(const panda::PFCand& cand) {
    TVector2 output;
    output.SetMagPhi(cand.pt(), cand.phi());
    return output;
  }
}

namespace trkmet {

  // Use TVector2::Mod() to get the MET
  // Use TVector2::Phi() to get phi
  TVector2 trkmet (const panda::Event& event) {
    TVector2 output;

    const auto& prim_vtx = event.vertices[0].position();

    for (auto& cand : event.pfCandidates) {
      if (cand.q() and std::abs(cand.dz(prim_vtx)) < 0.1)
        output += convert_pfcand(cand);
    }

    return output.Rotate(TMath::Pi());
  }

}

#endif
