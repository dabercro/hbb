#ifndef CROMBIE_LEPTONSELECT_H
#define CROMBIE_LEPTONSELECT_H 1


#include<cmath>
#include <vector>
#include <functional>

#include "PandaTree/Objects/interface/Event.h"

#include "reliso.h"
#include "roccor.h"


namespace {

  template <typename T>
    double correctedpt (const panda::Event& event, const T& lep);

  template <>
    double correctedpt<panda::Muon> (const panda::Event& event, const panda::Muon& lep) {
    return lep.pt() * roccor::scale(event, lep);
  }

  template <>
    double correctedpt<panda::Electron> (const panda::Event& event, const panda::Electron& lep) {
    return lep.smearedPt;
  }

}


namespace leptonselect {

  class LepInfo {
  public:
    LepInfo(const panda::Lepton* particle,
            double reliso,
            double minireliso,
            double corrpt,
            bool ismuon) :
      particle {particle},
      reliso {reliso},
      minireliso {minireliso},
      corrpt {corrpt},
      ismuon {ismuon} {}

    const panda::Lepton* particle;
    const double reliso;
    const double minireliso;
    const double corrpt;
    const bool ismuon;

  };

  class SelectedLeptons {
  public:
    SelectedLeptons (const panda::Event& event) {

      auto update = [this, &event] (const auto& leptons,
                                    bool ismuon,
                                    auto is_presel,
                                    auto is_loose,
                                    auto is_tight) {

        for (auto& lep : leptons) {
          auto reliso = lep.combIso()/lep.pt();
          auto minireliso = reliso::minireliso(lep, ismuon * event.rho);
          auto abseta = std::abs(lep.eta());

          auto check = [&] (auto& func) {
            return func(lep, reliso, minireliso, abseta);
          };

          if (not check(is_presel))
            continue;

          auto corrpt = correctedpt(event, lep);

          auto add = [&] (auto& vec) {
            vec.emplace_back(&lep, reliso, minireliso, corrpt, ismuon);
          };

          add(preselected);

          if (not check(is_loose))
            continue;

          add(loose);

          if (not check(is_tight))
            continue;

          add(tight);

        }

      };

      // Muons
      update(event.muons, true,
             [] (const panda::Muon& lep, double reliso, double minireliso, double abseta) {
               // Preselection
               return lep.pt() > 5.0 and abseta < 2.4 and lep.loose and
                 lep.dxy < 0.5 and lep.dz < 1.0 and
                 std::min(reliso, minireliso) < 0.4;
             },
             [] (const panda::Muon& lep, double reliso, double minireliso, double abseta) {
               // Loose
               return lep.pf and (lep.global or lep.tracker);
             },
             [] (const panda::Muon& lep, double reliso, double minireliso, double abseta) {
               // Tight
               return lep.tight and reliso < 0.15 and
                 lep.global and lep.normChi2 < 10.0 and
                 lep.nValidMuon > 0 and lep.nMatched > 1 and
                 lep.nValidPixel > 0 and lep.trkLayersWithMmt > 5 and
                 lep.dxy < 0.2 and lep.dz < 0.5;
             });

      // Electrons
      update(event.electrons, false,
             [] (const panda::Electron& lep, double reliso, double minireliso, double abseta) {
               // Preselection
               return lep.pt() > 7.0 and abseta < 2.4 and
                 lep.dxy < 0.05 and lep.dz < 0.20 and std::min(reliso, minireliso) < 0.4;
             },
             [] (const panda::Electron& lep, double reliso, double minireliso, double abseta) {
               // Loose
               return lep.mvaWP90;
             },
             [] (const panda::Electron& lep, double reliso, double minireliso, double abseta) {
               // Tight
               return lep.mvaWP80 and // Need to match selection applied for the training
                 lep.pt() > 15.0 and lep.hOverE < 0.09 and lep.trackIso/lep.pt() < 0.18 and
                 ((abseta < 1.4442 and lep.sieie < 0.012 and
                   lep.ecalIso/lep.pt() < 0.4 and lep.hcalIso/lep.pt() < 0.25 and
                   std::abs(lep.dEtaInSeed) < 0.0095 and std::abs(lep.dPhiIn) < 0.065) or
                  (abseta > 1.5660 and lep.sieie < 0.033 and
                   lep.ecalIso/lep.pt() < 0.45 and lep.hcalIso/lep.pt() < 0.28));             
             });

      // Taus
      for (auto& tau : event.taus) {
        if (tau.pt() > 18 && std::abs(tau.eta()) < 2.3 && tau.decayMode && tau.isoDeltaBetaCorr)
          numloosetau++;
      }

    }

    using leptonvec = std::vector<LepInfo>;

    leptonvec preselected {};
    leptonvec loose {};
    leptonvec tight {};

    unsigned numloosetau {0};

  };

}


#endif
