#ifndef REGRESSION_H
#define REGRESSION_H

#include <cmath>

#include "PandaTree/Objects/interface/Event.h"

#include "crombie/KinematicFunctions.h"

// Get all the variables for regression

namespace regression {

  const static unsigned pfmax = 10;

  class VectorWrapper : public TLorentzVector {
  public:

    VectorWrapper& operator += (const TLorentzVector& vec) {
      filled = true;
      TLorentzVector::operator+=(vec);
      return *this;
    }

    bool filled {false};

  };

  class PFInfo {
  public:

    PFInfo () {}
    PFInfo (float pt, float de, float dp, int pdg, float pup, float dxy, float dz) :
    ptfrac {pt}, deta {de}, dphi {dp}, pdgid {pdg}, puppiwt {pup}, dxy {dxy}, dz {dz} {}

    float ptfrac {};
    float deta {};
    float dphi {};
    int pdgid {};
    float puppiwt {};
    float dxy {};
    float dz {};

  };

  // Get the directions of the PUPPI particles in the area
  class JetInfo {
  public:
    using type = VectorWrapper;
    using scalar = float;

    // PUPPI vectors
    type charged {};
    type neutral {};
    type charged_pu {};
    type neutral_pu {};

    // Rings each are separate vector element
    using rings = std::vector<std::pair<scalar, type>>;
    rings em_rings = rings(6);
    rings neut_rings = rings(6);
    rings ch_rings = rings(6);
    rings mu_rings = rings(6);

    // Additional information about the pf candidates
    scalar ptD {0};
    unsigned num03 {0};

    std::vector<PFInfo> pfinfo = std::vector<PFInfo> (pfmax);

    // Vertex info
    unsigned vtxNtrk {0};
    float vtxMass {0};
    float vtx3dL {0};
    float vtx3deL {0};
    float vtxPt {0};

    // Max track

    double maxtrkpt {0};
    double maxpfpt {0};

    // Lepton stuff

    int leptonPdgId {0};
    double leptonPt {0};
    double leptonPtRel {0};
    double leptonPtRelInv {0};
    double leptonDeltaR {0};

  };



  JetInfo GetJetInfo(const panda::Jet& jet) {
    JetInfo output {};

    // For the ptD calculation
    double sum_weight {0};
    double sum_pt {0};

    // Fill information about the constituents
    unsigned pfindex = 0;
    for (auto pf : jet.constituents) {
      if (not pf.isValid())
        continue;

      auto p4 = pf->p4();

      // PUPPI
      auto puppi = pf->puppiW();
      if (pf->q()) {
        output.charged += p4 * puppi;
        output.charged_pu += p4 * (1.0 - puppi);
      }
      else {
        output.neutral += p4 * puppi;
        output.neutral_pu += p4 * (1.0 - puppi);
      }

      // Energy Rings
      static const std::vector<double> dr_bins {
        std::pow(0.05, 2), std::pow(0.1, 2), std::pow(0.2, 2), std::pow(0.3, 2), std::pow(0.4, 2), std::pow(0.5, 2)
      };
      unsigned bin = std::lower_bound(dr_bins.begin(), dr_bins.end(),
                                      deltaR2(p4.Eta(), p4.Phi(), jet.eta(), jet.phi())) - dr_bins.begin();
      if (bin < dr_bins.size()) {
        // Get which ring offset
        auto rings = [&pf] () {
          unsigned pdgid = std::abs(pf->pdgId());
          if (pdgid == 22 || pdgid == 11)
            return &JetInfo::em_rings;
          else if (pdgid == 13)
            return &JetInfo::mu_rings;
          else if (pf->q())
            return &JetInfo::ch_rings;
          return &JetInfo::neut_rings;
        } ();
        // Apply info
        (output.*rings)[bin].first += p4.E();
        (output.*rings)[bin].second += p4;
      }

      // ptD
      auto pt = p4.Pt();
      sum_weight += std::pow(pt, 2);
      sum_pt += pt;
      // num candidates with pt > 0.3
      if (pt > 0.3)
        ++output.num03;

      if (pfindex < pfmax) {
        auto trk = pf->track;
        output.pfinfo[pfindex++] = PFInfo(pt/jet.pt(),
                                          p4.Eta() - jet.eta(),
                                          TVector2::Phi_mpi_pi(p4.Phi() - jet.phi()),
                                          pf->pdgId(),
                                          puppi,
                                          trk.isValid() ? trk->dxy() : -1.0,
                                          trk.isValid() ? trk->dz() : -1.0);
      }

    }

    output.ptD = std::sqrt(sum_weight)/sum_pt;

    // Secondary vertex

    auto& vert = jet.secondaryVertex;
    if (vert.isValid()) {
      output.vtxNtrk = vert->ntrk;
      output.vtxMass = vert->m();
      output.vtx3dL = vert->vtx3DVal;
      output.vtx3deL = vert->vtx3DeVal;
      output.vtxPt = vert->pt();
    }

    // Max track

    const panda::PFCand* maxlep = nullptr;

    for (auto pf : jet.constituents) {
      if (pf.isValid()) {
        auto pt = pf->pt();
        output.maxpfpt = std::max(output.maxpfpt, pt);
        if (pf->q())
          output.maxtrkpt = std::max(output.maxtrkpt, pt);

        auto pdgid = abs(pf->pdgId());
        if (pdgid == 13 || pdgid == 15) {
          if (not maxlep or pt > maxlep->pt())
            maxlep = pf.get();
        }
      }
    }

    if (maxlep) {
      auto lep_p4 = maxlep->p4();
      auto jet_p4 = jet.p4();
      output.leptonPdgId = maxlep->pdgId();
      output.leptonPt = maxlep->pt();
      output.leptonPtRel = lep_p4.Perp(jet_p4.Vect());
      output.leptonPtRelInv = jet_p4.Perp(lep_p4.Vect());
      output.leptonDeltaR = std::sqrt(deltaR2(jet.eta(), jet.phi(),
                                              maxlep->eta(), maxlep->phi()));
    }

    return output;
  }
}


#endif
