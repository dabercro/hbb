#include <algorithm>
#include <iostream>
#include <functional>
#include <initializer_list>

#include "checkrun.h"
#include "hbbfile.h"
#include "feedpanda.h"

#include "SkimmingTools/interface/CmsswParse.h"
#include "SkimmingTools/interface/StoreParticles.h"
#include "PandaTree/Objects/interface/Event.h"

#include "TH1F.h"

int parsed_main(int argc, char** argv) {

  hbbfile output {argv[argc - 1]};
  TH1F all_hist {"htotal", "htotal", 1, -1, 1};

  // Loop over all input files
  for (int i_file = 1; i_file < argc - 1; i_file++) {

    std::cout << "Running over file " << argv[i_file]
              << " (" << i_file << "/" << (argc - 2) << ")" << std::endl;

    // Get the PandaTree
    TFile input {argv[i_file]};
    auto* events_tree = static_cast<TTree*>(input.Get("events"));
    panda::Event event;
    feedpanda(event, events_tree);
    auto nentries = events_tree->GetEntries();

    //// TRIGGERS ////

    const std::vector<const char*> met_trigger_paths = {
      "HLT_PFMET110_PFMHT110_IDTight",
      "HLT_PFMET120_PFMHT120_IDTight",
      "HLT_PFMET170_NoiseCleaned",
      "HLT_PFMET170_HBHECleaned",
      "HLT_PFMET170_JetIdCleaned",
      "HLT_PFMET170_NotCleaned",
      "HLT_PFMET170_HBHE_BeamHaloCleaned"
    };

    std::vector<unsigned> met_trigger_tokens;

    for (auto path : met_trigger_paths)
      met_trigger_tokens.push_back(event.registerTrigger(path));

    // Loop over tree
    for(decltype(nentries) entry = 0; entry != nentries; ++entry) {
      if (entry % 10000 == 0)
        std::cout << "Processing events: ... " << float(entry)/nentries*100 << " %" << std::endl;
      event.getEntry(*events_tree, entry);
      output.reset(event);

      all_hist.Fill(0.0, (event.weight < 0) ? -1.0 : 1.0);

      //// FILTER OUT LOW MET AND GOODRUNS ////

      if (output.met < 70 || not checkrun(event.runNumber, event.lumiNumber))
        continue;

      // Check triggers
      for (auto token : met_trigger_tokens) {
        if (event.triggerFired(token)) {
          output.met_trigger = true;
          break;
        }
      }

      //// PHOTONS ////
      for (auto& pho : event.photons) {
        if (pho.loose)
          output.n_loosepho++;
      }

      //// TAUS ////
      for (auto& tau : event.taus) {
        if (tau.pt() > 18 && fabs(tau.eta()) < 2.3 && tau.decayMode && tau.isoDeltaBetaCorr)
          output.n_loosetau++;
      }

      //// OTHER LEPTONS ////

      TLorentzVector lepvec {};

      using lepstore = ObjectStore<hbbfile::lep, panda::Lepton, int>;
      lepstore stored_muons({hbbfile::lep::muon1, hbbfile::lep::muon2}, [](panda::Lepton* l) {return l->pt();});
      lepstore stored_eles({hbbfile::lep::ele1, hbbfile::lep::ele2}, [](panda::Lepton* l) {return l->pt();});

      // Define default checks here
      // We'll do a lambda with no arguments for lazy evaluation of lepton IDs
      using lazy_id = std::function<bool()>;

      auto check_lep = [&output, &lepvec] (lepstore& store, panda::Lepton& lep,
                                           lazy_id is_loose, lazy_id is_med, lazy_id is_tight) {
        if (is_loose()) {
          int stat_flag = 1;
          lepvec += lep.p4();
          output.n_looselep++;
          if (is_med()) {
            output.n_mediumlep++;
            stat_flag++;
            if (is_tight()) {
              output.n_tightlep++;
              stat_flag++;
            }
          }
          store.check(lep, stat_flag);
        }
      };

      // Loop over muons
      for (auto& lep : event.muons) {
        check_lep(stored_muons, lep,
                  [&] {   // Loose muons
                    return lep.loose and lep.pt() >= 10 and fabs(lep.eta()) <= 2.4;
                  },
                  [&] {   // Medium muons
                    return lep.medium and (lep.combIso()/lep.pt()) < 0.15;
                  },
                  [&] {   // Tight muons
                    return lep.tight;
                  });
      }

      // Loop over electrons
      for (auto& lep : event.electrons) {
        auto abseta = fabs(lep.eta());
        auto isoscale = abseta < 1.4442 ? 2 : 1;
        check_lep(stored_eles, lep,
                  [&] {   // Loose electrons
                    return lep.veto and lep.pt() >= 10 and fabs(lep.eta()) <= 2.5 and
                      lep.dxy < (0.10/isoscale) and lep.dz < (0.20/isoscale);
                  },
                  [&] {   // Medium electrons
                    return lep.medium;
                  },
                  [&] {   // Tight electrons
                    return lep.tight;
                  });
      }

      auto set_lep = [&output] (std::initializer_list<lepstore> stores) {
        for (auto& leps : stores) {
          for (auto& lep : leps.store) {
            if (not lep.particle)
              break;
            output.set_lep(lep.branch, *lep.particle, lep.extra);
          }
        }
      };

      set_lep({stored_muons, stored_eles});

      //// JETS ////

      // We want the two jets with the highest CSV and CMVA
      using jetstore = ObjectStore<hbbfile::jet, panda::Jet>;

      jetstore stored_jets({hbbfile::jet::jet1, hbbfile::jet::jet2, hbbfile::jet::jet3},
                           [](panda::Jet* j) {return j->pt();});
      jetstore stored_csvs({hbbfile::jet::csv_jet1, hbbfile::jet::csv_jet2},
                           [](panda::Jet* j) {return j->csv;});
      jetstore stored_cmvas({hbbfile::jet::cmva_jet1, hbbfile::jet::cmva_jet2},
                            [](panda::Jet* j) {return j->cmva;});

      for (auto& jet : event.chsAK4Jets) {

        if (jet.pt() < 20.0)
          continue;

        // Count all jets (including forward)
        output.n_alljet++;
        if (fabs(jet.eta()) > 2.5)
          continue;

        output.n_jet++;
        output.min_dphi_metj_soft = std::min(output.min_dphi_metj_soft, deltaPhi(output.metphi, jet.phi()));

        if (jet.pt() > 30.0) {
          output.n_hardjet++;
          output.min_dphi_metj_hard = std::min(output.min_dphi_metj_hard, deltaPhi(output.metphi, jet.phi()));
          stored_csvs.check(jet);
          stored_cmvas.check(jet);
        }

        stored_jets.check(jet);
      }

      auto set_jet = [&output] (std::initializer_list<jetstore> stores) {
        for (auto& jets : stores) {
          for (auto& jet : jets.store) {
            if (not jet.particle)
              break;
            output.set_jet(jet.branch, *jet.particle);
            auto& gen = jet.particle->matchedGenJet;
            if (gen.isValid())
              output.set_genjet(jet.branch, *gen);
          }
        }
      };

      set_jet({stored_jets});

      // Includes getting secondary vertex and leading leptons
      auto set_bjet = [&output, &set_jet] (std::initializer_list<jetstore> stores) {
        set_jet(stores);
        for (auto& jets : stores) {
          for (auto& jet : jets.store) {
            if (not jet.particle)
              break;

            auto bjet = to_bjet(jet.branch);
            output.set_bjet(bjet, *jet.particle);

            auto& vert = jet.particle->secondaryVertex;
            if (vert.isValid())
              output.set_bvert(bjet, *vert);

            int nlep = 0;
            const panda::PFCand* maxlep = nullptr;

            decltype(maxlep->pt()) maxtrkpt = 0;

            for (auto pf : jet.particle->constituents) {
              if (pf->q()) {
                auto pt = pf->pt();
                maxtrkpt = std::max(maxtrkpt, pt);
                auto pdgid = abs(pf->pdgId());
                if (pdgid == 11 || pdgid == 13) {
                  nlep++;
                  if (not maxlep or pt > maxlep->pt())
                    maxlep = &*pf;    // Dereference the panda::Ref, and then get the address
                }
              }
            }

            output.set_bmaxtrk(bjet, maxtrkpt);
            if (maxlep)
              output.set_bleps(bjet, *jet.particle, nlep, *maxlep);
          }
        }
      };

      set_bjet({stored_csvs, stored_cmvas});

      if (output.csv_jet2_csv > 0.3)
        output.set_hbb(hbbfile::hbb::csv_hbb, stored_csvs.store[0].particle->p4() + stored_csvs.store[1].particle->p4());
      if (output.cmva_jet2_cmva > -0.7)
        output.set_hbb(hbbfile::hbb::cmva_hbb, stored_cmvas.store[0].particle->p4() + stored_cmvas.store[1].particle->p4());

      //// FILTER ////

      if (not (output.csv_hbb or output.cmva_hbb))
        continue;

      //// GEN BOSON FOR KFACTORS AND TTBAR FOR PT SCALING ////
      for (auto& gen : event.genParticles) {
        auto abspdgid = abs(gen.pdgid);
        if (not output.genboson and (abspdgid == 23 or abspdgid == 24))
          output.set_gen(hbbfile::gen::genboson, gen);

        else if (not output.gen_t and gen.pdgid == 6)
          output.set_gen(hbbfile::gen::gen_t, gen);

        else if (not output.gen_tbar and gen.pdgid == -6)
          output.set_gen(hbbfile::gen::gen_tbar, gen);
      }

      //// RECOIL ////

      auto recoilvec = event.pfMet.v() + lepvec.Vect().XYvector();
      output.recoil = recoilvec.Mod();
      output.recoilphi = recoilvec.Phi();

      output.dphi_uh_csv = deltaPhi(output.csv_hbb_phi, output.recoilphi);
      output.dphi_uh_cmva = deltaPhi(output.cmva_hbb_phi, output.recoilphi);

      output.fill();
    }
    input.Close();
  }

  output.write(&all_hist);
  return 0;
}

int main(int argc, char** argv) {
  return parse_then_send(argc, argv, parsed_main);
}
