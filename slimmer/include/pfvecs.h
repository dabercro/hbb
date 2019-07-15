#ifndef PFVECS_H
#define PFVECS_H

#include <vector>

#include "fastjet/ClusterSequence.hh"

#include "PandaTree/Objects/interface/Event.h"

namespace pfvecs {

  class PFVecs {
  public:
    PFVecs (unsigned size) :
      pf_pt_sorted_pt (size),
      pf_pt_sorted_eta (size),
      pf_pt_sorted_phi (size),
      pf_pt_sorted_m (size),
      pf_pt_sorted_dxy (size),
      pf_pt_sorted_dz (size),
      pf_pt_sorted_q (size),
      pf_fastjet_sorted_pt (size),
      pf_fastjet_sorted_eta (size),
      pf_fastjet_sorted_phi (size),
      pf_fastjet_sorted_m (size),
      pf_fastjet_sorted_dxy (size),
      pf_fastjet_sorted_dz (size),
      pf_fastjet_sorted_q (size) {}

    std::vector<float> pf_pt_sorted_pt;
    std::vector<float> pf_pt_sorted_eta;
    std::vector<float> pf_pt_sorted_phi;
    std::vector<float> pf_pt_sorted_m;
    std::vector<float> pf_pt_sorted_dxy;
    std::vector<float> pf_pt_sorted_dz;
    std::vector<char> pf_pt_sorted_q;

    std::vector<float> pf_fastjet_sorted_pt;
    std::vector<float> pf_fastjet_sorted_eta;
    std::vector<float> pf_fastjet_sorted_phi;
    std::vector<float> pf_fastjet_sorted_m;
    std::vector<float> pf_fastjet_sorted_dxy;
    std::vector<float> pf_fastjet_sorted_dz;
    std::vector<char> pf_fastjet_sorted_q;
  };

  PFVecs get_vecs(const panda::Jet& jet, TVector3 primary_vert) {
    PFVecs output {jet.constituents.size()};

    unsigned i_pt {0};

    // Make pseudojets

    std::vector<fastjet::PseudoJet> pseudojets;
    pseudojets.reserve(jet.constituents.size());

    for (auto cand : jet.constituents) {
      output.pf_pt_sorted_pt[i_pt] = cand->pt();
      output.pf_pt_sorted_eta[i_pt] = cand->eta();
      output.pf_pt_sorted_phi[i_pt] = cand->phi();
      output.pf_pt_sorted_m[i_pt] = cand->m();
      output.pf_pt_sorted_dxy[i_pt] = cand->dxy(primary_vert);
      output.pf_pt_sorted_dz[i_pt] = cand->dz(primary_vert);
      output.pf_pt_sorted_q[i_pt] = cand->q();
      i_pt++;

      // Add pseudojets
      pseudojets.emplace_back(cand->px(), cand->py(), cand->pz(), cand->e());
    }

    auto sequence = fastjet::ClusterSequence(pseudojets, {fastjet::JetAlgorithm::antikt_algorithm, 0.4});

    unsigned i_fastjet {0};

    for (auto i_cand : sequence.unique_history_order()) {
      if (i_cand >= jet.constituents.size())
        continue;

      output.pf_fastjet_sorted_pt[i_fastjet] = jet.constituents.at(i_cand)->pt();
      output.pf_fastjet_sorted_eta[i_fastjet] = jet.constituents.at(i_cand)->eta();
      output.pf_fastjet_sorted_phi[i_fastjet] = jet.constituents.at(i_cand)->phi();
      output.pf_fastjet_sorted_m[i_fastjet] = jet.constituents.at(i_cand)->m();
      output.pf_fastjet_sorted_dxy[i_fastjet] = jet.constituents.at(i_cand)->dxy(primary_vert);
      output.pf_fastjet_sorted_dz[i_fastjet] = jet.constituents.at(i_cand)->dz(primary_vert);
      output.pf_fastjet_sorted_q[i_fastjet] = jet.constituents.at(i_cand)->q();

      if (++i_fastjet == jet.constituents.size())
        break;
    }

    return output;
  }

}

#endif
