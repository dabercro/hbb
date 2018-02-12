#include <cmath>
#include <algorithm>
#include <iostream>
#include <functional>

#include "checkrun.h"
#include "hbbfile.h"
#include "feedpanda.h"
#include "btagreaders.h"

#include "SkimmingTools/interface/CmsswParse.h"
#include "SkimmingTools/interface/StoreParticles.h"
#include "PandaTree/Objects/interface/Event.h"
#include "PlotTools/interface/KinematicFunctions.h"

#include "TH1F.h"

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

int parsed_main(int argc, char** argv) {

  constexpr BTagCounter csv_counter {0.5426, 0.8484, 0.9535};
  constexpr BTagCounter cmva_counter {-0.5884, 0.4432, 0.9432};

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

      all_hist.Fill(0.0, output.mc_weight);

      //// FILTER ////
      if (not checkrun(event.runNumber, event.lumiNumber))
        continue;

      // Check triggers
      for (auto token : met_trigger_tokens) {
        if (event.triggerFired(token)) {
          output.met_trigger = true;
          break;
        }
      }

      //// PHOTONS ////

      std::vector<std::pair<float, float>> em_directions; // Pairs of eta, phi for all leptons and photons for cleaning

      for (auto& pho : event.photons) {
        if (pho.loose) {
          output.n_pho_loose++;
          if (pho.medium && pho.pt() > 175)
            em_directions.emplace_back(pho.eta(), pho.phi());
        }
      }

      //// TAUS ////
      for (auto& tau : event.taus) {
        if (tau.pt() > 18 && fabs(tau.eta()) < 2.3 && tau.decayMode && tau.isoDeltaBetaCorr)
          output.n_tau_loose++;
      }

      //// OTHER LEPTONS ////

      TLorentzVector lepvec {};

      using lepstore = ObjectStore<hbbfile::lep, panda::Lepton, int>;
      lepstore stored_muons({hbbfile::lep::muon1, hbbfile::lep::muon2}, [](panda::Lepton* l) {return l->pt();});
      lepstore stored_eles({hbbfile::lep::ele1, hbbfile::lep::ele2}, [](panda::Lepton* l) {return l->pt();});

      // Define default checks here
      // We'll do a lambda with no arguments for lazy evaluation of lepton IDs
      using lazy_id = std::function<bool()>;

      auto check_lep = [&output, &lepvec, &em_directions] (lepstore& store, panda::Lepton& lep,
                                                           lazy_id is_loose, lazy_id is_med, lazy_id is_tight) {
        if (is_loose()) {
          int stat_flag = 1;
          lepvec += lep.p4();
          output.n_lep_loose++;
          em_directions.emplace_back(lep.eta(), lep.phi());
          if (is_med()) {
            output.n_lep_medium++;
            stat_flag++;
            if (is_tight()) {
              output.n_lep_tight++;
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

      //// RECOIL ////
      // I inserted in here to use the lepvec for filtering

      auto recoilvec = event.pfMet.v() + lepvec.Vect().XYvector();

      //// BACK TO LEP ////

      auto set_lep = [&output] (std::vector<lepstore> stores) {
        for (auto& leps : stores) {
          for (auto& lep : leps.store) {
            if (not lep.particle)
              break;
            output.set_lep(lep.branch, *lep.particle, lep.extra);
          }
        }
      };

      set_lep({stored_muons, stored_eles});

      //// GEN PARTICLES ////

      using genhstore = ObjectStore<hbbfile::hbb, panda::GenParticle>;

      // Get the generator particles that are closest to the reconstructed Higgs

      std::vector<genhstore> gen_higgs {
        {{hbbfile::hbb::cmva_hbb}, [&output] (panda::GenParticle* gen) {return deltaR2(output.cmva_hbb_eta, output.cmva_hbb_phi, gen->eta(), gen->phi());}, genhstore::order::eAsc}
      };

      std::map<const panda::GenJet*, GenNuVec> gen_nu_map;

      //// GEN BOSON FOR KFACTORS AND TTBAR FOR PT SCALING ////
      for (auto& gen_jet : event.ak4GenJets) {
        if (gen_jet.pt() > 20 && fabs(gen_jet.eta()) < 2.4 && gen_jet.numB != 0)
          ++output.n_genB;
      }

      for (auto& gen : event.genParticles) {
        auto abspdgid = abs(gen.pdgid);
        if (not output.genboson and (abspdgid == 23 or abspdgid == 24))
          output.set_gen(hbbfile::gen::genboson, gen);

        else if (not output.gen_t and gen.pdgid == 6)
          output.set_gen(hbbfile::gen::gen_t, gen);

        else if (not output.gen_tbar and gen.pdgid == -6)
          output.set_gen(hbbfile::gen::gen_tbar, gen);

        else if (abspdgid == 25) {
          for (auto& store : gen_higgs)
            store.check(gen);
        }

        else if (abspdgid == 12 or abspdgid == 14) {
          // Check jets of each collection for closest jet to neutrinos and add to the genvec stored in jetstore's extra
          float cone_size = std::pow(0.4, 2);   // Neutrinos must be within deltaR2 = (0.4)^2
          panda::GenJet* closest = nullptr;
          for (auto& gen_jet : event.ak4GenJets) {
            auto check = deltaR2(gen_jet.eta(), gen_jet.phi(), gen.eta(), gen.phi());
            // If the neutrino momentum is super high, probably not from this jet, so scale by anti-kt metric
            if (gen.pt() > gen_jet.pt())
              check *= pow(gen.pt()/gen_jet.pt(), 2);
            if (check < cone_size) {
              cone_size = check;
              closest = &gen_jet;
            }
          }
          if (closest) {
            if (gen_nu_map.find(closest) == gen_nu_map.end())
              gen_nu_map[closest] = {closest->p4()};
            gen_nu_map[closest].add_nu(gen);
          }
        }
      }

      for (auto& gen_higg : gen_higgs) {
        for (auto& entry : gen_higg.store) {
          if (entry.particle)
            output.set_hbbgen(entry.branch, *entry.particle, sqrt(entry.result));
        }
      }

      //// JETS ////

      // We want the two jets with the highest CSV and CMVA, and carry along the calibrator
      using jetstore = ObjectStore<hbbfile::jet, panda::Jet, const BCalReaders*>;

      jetstore stored_jets({hbbfile::jet::jet1, hbbfile::jet::jet2, hbbfile::jet::jet3},
                           [](panda::Jet* j) {return j->pt();});
      jetstore stored_cmvas({hbbfile::jet::cmva_jet1, hbbfile::jet::cmva_jet2, hbbfile::jet::cmva_jet3},
                            [](panda::Jet* j) {return j->cmva;});

      // Check if overlaps with EM object
      auto overlap_em = [&em_directions] (panda::Jet& jet) {
        for (auto& dir : em_directions) {
          if (deltaR2(jet.eta(), jet.phi(), dir.first, dir.second) < pow(0.4, 2))  // Within dR = 0.4
            return true;
        }
        return false;
      };

      for (auto& jet : event.chsAK4Jets) {
        if (overlap_em(jet) || jet.pt() < 20.0 || not jet.loose)
          continue;

        // Count all jets (including forward)
        output.n_alljet++;
        stored_jets.check(jet);

        if (fabs(jet.eta()) > 2.5)
          continue;

        output.n_jet++;
        output.min_dphi_metj_soft = std::min(output.min_dphi_metj_soft, deltaPhi(output.metphi, jet.phi()));

        if (fabs(jet.eta()) < 2.4) {
          csv_counter.count(jet.csv, output.n_bcsv_loose, output.n_bcsv_medium, output.n_bcsv_tight);
          cmva_counter.count(jet.cmva, output.n_bcmva_loose, output.n_bcmva_medium, output.n_bcmva_tight);
          stored_cmvas.check(jet, &cmva_readers);
        }

        if (jet.pt() > 30.0) {
          output.n_hardjet++;
          output.min_dphi_metj_hard = std::min(output.min_dphi_metj_hard, deltaPhi(output.metphi, jet.phi()));
        }

      }

      auto set_jet = [&output, &gen_nu_map] (std::vector<jetstore*> stores) {
        for (auto jets : stores) {
          for (auto& jet : jets->store) {
            if (not jet.particle)
              break;
            output.set_jet(jet.branch, *jet.particle);
            auto& gen = jet.particle->matchedGenJet;
            if (gen.isValid()) {
              const auto& gennu = gen_nu_map.find(&*gen) != gen_nu_map.end() ? gen_nu_map[&*gen] : GenNuVec(gen->p4());
              output.set_genjet(jet.branch, *gen, gennu.genvec, gennu.numnu, gennu.overlap);
            }
          }
        }
      };

      set_jet({&stored_jets});

      // Includes getting secondary vertex and leading leptons
      auto set_bjet = [&output, &set_jet] (std::vector<jetstore*> stores) {
        set_jet(stores);
        for (auto jets : stores) {
          for (auto& jet : jets->store) {
            if (not jet.particle)
              break;

            auto bjet = to_bjet(jet.branch);

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

            // Determine the flavor of the jet
            auto flavor = BTagEntry::FLAV_UDSG;
            auto& gen = jet.particle->matchedGenJet;
            if (gen.isValid()) {
              auto abspdgid = abs(gen->pdgid);
              if (abspdgid == 5)
                flavor = BTagEntry::FLAV_B;
              else if (abspdgid == 4)
                flavor = BTagEntry::FLAV_C;
            }

            output.set_bjet(bjet, *jet.particle, maxtrkpt, *jet.extra, flavor);
            if (maxlep)
              output.set_bleps(bjet, *jet.particle, nlep, *maxlep);
          }
        }
      };

      // set_bjet({&stored_csvs, &stored_cmvas});
      set_bjet({&stored_cmvas});

      if (output.cmva_jet2_cmva > -2.0)
        output.set_hbb(hbbfile::hbb::cmva_hbb, stored_cmvas.store[0].particle->p4() + stored_cmvas.store[1].particle->p4());

      output.fill(recoilvec);
    }
    input.Close();
  }

  output.write(&all_hist);
  return 0;
}

int main(int argc, char** argv) {
  return parse_then_send(argc, argv, parsed_main);
}
