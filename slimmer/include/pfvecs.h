#ifndef PFVECS_H
#define PFVECS_H

#include <vector>

#include "TLorentzRotation.h"

#include "fastjet/ClusterSequence.hh"

#include "PandaTree/Objects/interface/Event.h"


namespace pfvecs {

  TLorentzRotation get_transform (TLorentzVector jet,
                                  TLorentzVector pf_0,
                                  TLorentzVector pf_1) {

    TLorentzRotation output {};

    // Boost into jet's center of mass
    output.Boost(-1 * jet.BoostVector());

    // Rotate around z until desired pf_0 in x-z plane
    auto z_direction = output * pf_0;
    output.RotateZ(-1 * z_direction.Phi());

    // Rotate around y until in z direction
    output.RotateY(-1 * z_direction.Theta());

    // Now rotate pf_1 into x-z plane, after applying transform so far
    auto xz_plane = output * pf_1;
    output.RotateZ(-1 * xz_plane.Phi());

    // Done!
    return output;

  }

  class PFInfo {
  public:
    PFInfo (const panda::Jet& jet,
            const panda::PFCand& pfcand,
            const TVector3& pv,
            TLorentzRotation transform) : 
      ptfrac {pfcand.pt()/jet.pt()},
      deta {pfcand.eta() - jet.eta()},
      dphi {pfcand.phi() - jet.phi()},
      puppiwt {pfcand.puppiW()},
      dxy {pfcand.dxy(pv)},
      dz {pfcand.dz(pv)},
      q {pfcand.q()},
      is_ele {pfcand.ptype == panda::PFCand::PType::ep or pfcand.ptype == panda::PFCand::PType::em},
      is_muon {pfcand.ptype == panda::PFCand::PType::mup or pfcand.ptype == panda::PFCand::PType::mum},
      is_photon {pfcand.ptype == panda::PFCand::PType::gamma},
      is_nhadron {pfcand.ptype == panda::PFCand::PType::h0},
      is_chhadron {pfcand.ptype == panda::PFCand::PType::hp or pfcand.ptype == panda::PFCand::PType::hm},
      transformed {transform * pfcand.p4()}
    {}

    double ptfrac;
    double deta;
    double dphi;
    double puppiwt;
    double dxy;
    double dz;
    int q;

    bool is_ele;
    bool is_muon;
    bool is_photon;
    bool is_nhadron;
    bool is_chhadron;

    TLorentzVector transformed;

  };

  std::vector<PFInfo> get_vecs(const panda::Jet& jet, TVector3 primary_vert) {
    std::vector<PFInfo> output {};
    output.reserve(jet.constituents.size());

    if (jet.constituents.size() < 2)
      // Return empty stuff
      return output;

    auto transform = get_transform(jet.p4(), jet.constituents.at(0)->p4(), jet.constituents.at(1)->p4());

    // Make pseudojets

    std::vector<fastjet::PseudoJet> pseudojets;
    pseudojets.reserve(jet.constituents.size());

    // Add pseudojets
    for (auto cand : jet.constituents)
      pseudojets.emplace_back(cand->px(), cand->py(), cand->pz(), cand->e());

    auto sequence = fastjet::ClusterSequence(pseudojets, {fastjet::JetAlgorithm::antikt_algorithm, 0.6});

    for (auto i_cand : sequence.unique_history_order()) {
      if (i_cand >= jet.constituents.size())
        continue;

      output.emplace_back(jet, *jet.constituents.at(i_cand), primary_vert, transform);
    }

    return output;
  }

}

#endif
