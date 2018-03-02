#include <cmath>
#include <iostream>
#include <functional>

#include "checkrun.h"
#include "feedpanda.h"
#include "btagreaders.h"
#include "hbbfile.h"
#include "misc.h"
#include "debugevent.h"

#include "fastjet/ClusterSequence.hh"

#include "SkimmingTools/interface/CmsswParse.h"
#include "SkimmingTools/interface/ObjectStore.h"

#include "TH1F.h"

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
      "HLT_PFMET170_NoiseCleaned",
      "HLT_PFMET170_HBHECleaned",
      "HLT_PFMET170_JetIdCleaned",
      "HLT_PFMET170_NotCleaned",
      "HLT_PFMET170_HBHE_BeamHaloCleaned",
      "HLT_PFMETNoMu120_NoiseCleaned_PFMHTNoMu120_IDTight",
      "HLT_PFMETNoMu110_NoiseCleaned_PFMHTNoMu110_IDTight",
      "HLT_PFMETNoMu90_NoiseCleaned_PFMHTNoMu90_IDTight",
      "HLT_PFMETNoMu90_PFMHTNoMu90_IDTight",
      "HLT_PFMETNoMu100_PFMHTNoMu100_IDTight",
      "HLT_PFMETNoMu110_PFMHTNoMu110_IDTight",
      "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"
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

      if (debug::debug) {
        if (debug::check(event.runNumber, event.lumiNumber, event.eventNumber)) {
          std::cout << std::endl << "Found Event in row " << entry << std::endl << std::endl;
          for (auto token : met_trigger_tokens) {
            if (event.triggerFired(token))
              std::cout << met_trigger_paths[token] << std::endl;
          }
          event.pfMet.dump();
          event.photons.dump();
          event.muons.dump();
          event.electrons.dump();
          event.chsAK4Jets.dump();
        }
        else 
          continue;
      }

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

      for (auto& pho : event.photons) {
        output.n_pho_loose += pho.loose;
        output.n_pho_medium += pho.medium;
        output.n_pho_tight += pho.tight;
      }

      //// TAUS ////
      for (auto& tau : event.taus) {
        if (tau.pt() > 18 && std::abs(tau.eta()) < 2.3 && tau.decayMode && tau.isoDeltaBetaCorr)
          output.n_tau_loose++;
      }

      //// OTHER LEPTONS ////

      std::vector<std::pair<float, float>> em_directions; // Pairs of eta, phi for preselected leptons for cleaning

      TLorentzVector lepvec {};

      using lepstore = ObjectStore<hbbfile::lep, panda::Lepton, LepInfo>;
      lepstore stored_muons({hbbfile::lep::muon1, hbbfile::lep::muon2}, [](panda::Lepton* l) {return l->pt();});
      lepstore stored_eles({hbbfile::lep::ele1, hbbfile::lep::ele2}, [](panda::Lepton* l) {return l->pt();});

      // Define default checks here
      // We'll do a lambda with no arguments for lazy evaluation of lepton IDs
      using lazy_id = std::function<bool()>;

      auto check_lep = [&output, &lepvec, &em_directions] (lepstore& store, panda::Lepton& lep, float reliso,
                                                           lazy_id preselection, lazy_id is_loose, lazy_id is_med, lazy_id is_tight) {
        // Definitions straight from AN
        if (preselection()) {
          LepInfo::SelectionFlag stat_flag = LepInfo::SelectionFlag::presel;
          em_directions.emplace_back(lep.eta(), lep.phi());
          if (is_loose()) {
            lepvec += lep.p4();
            output.n_lep_loose++;
            stat_flag = LepInfo::SelectionFlag::loose;
            if (is_med()) {
              output.n_lep_medium++;
              stat_flag = LepInfo::SelectionFlag::medium;
              if (is_tight()) {
                output.n_lep_tight++;
                stat_flag = LepInfo::SelectionFlag::tight;
              }
            }
            store.check(lep, {stat_flag, reliso});  // Only want to store leptons that are at least loose
          }
        }

        // POG definitions
        // Need kinematics cuts
        output.n_lep_pog_loose += lep.loose;
        output.n_lep_pog_medium += lep.medium;
        output.n_lep_pog_tight += lep.tight;
      };

      // Loop over muons
      for (auto& lep : event.muons) {

        auto abseta = std::abs(lep.eta());
        auto reliso = (lep.chIso + lep.nhIso + lep.phIso - 0.5 * lep.puIso)/lep.pt();

        check_lep(stored_muons, lep, reliso,
                  [&] {   // Muon preselection
                    return lep.pt() > 5.0 and abseta < 2.4 and
                      lep.dxy < 0.5 and lep.dz < 1.0 and reliso < 0.4;
                  },
                  [&] {   // Loose muons
                    return lep.pf and (lep.global or lep.tracker);
                  },
                  [&] {   // Medium muons; they don't seem to do anything with those
                    return true;
                  },
                  [&] {   // Tight muons; there's probably also a pT cut, but they don't give it directly
                    return lep.global and lep.normChi2 < 10.0 and
                      lep.nValidMuon > 0 and lep.nMatched > 1 and
                      lep.nValidPixel > 0 and lep.trkLayersWithMmt > 5 and
                      lep.dxy < 0.2 and lep.dz < 0.5;
                  });
      }

      // Loop over electrons
      for (auto& lep : event.electrons) {

        auto abseta = std::abs(lep.eta());
        auto reliso = (lep.chIso + lep.nhIso + lep.phIso - lep.isoPUOffset)/lep.pt();

        check_lep(stored_eles, lep, reliso,
                  [&] {   // Electron preselection
                    return lep.pt() > 7.0 and abseta < 2.4 and
                      lep.dxy < 0.05 and lep.dz < 0.20 and
                      reliso < 0.4;
                  },
                  [&] {   // Loose electrons for conservative event classification
                    return lep.mvaWP90;
                  },
                  [&] {   // "Medium" electrons, which I define as the cut applied to match MVA training
                    return lep.pt() > 15.0 and lep.hOverE < 0.09 and reliso < 0.18 and
                      ((abseta < 1.4442 and lep.sieie < 0.012 and
                        lep.ecalIso/lep.pt() < 0.4 and lep.hcalIso/lep.pt() < 0.25 and
                        std::abs(lep.dEtaInSeed) < 0.0095 and std::abs(lep.dPhiIn) < 0.065) or
                       (abseta > 1.5660 and lep.sieie < 0.033 and
                        lep.ecalIso/lep.pt() < 0.45 and lep.hcalIso/lep.pt() < 0.28)
                       );
                  },
                  [&] {   // Tight electrons
                    return lep.mvaWP80;
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
        if (gen_jet.pt() > 20 && std::abs(gen_jet.eta()) < 2.4 && gen_jet.numB != 0)
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
              check *= std::pow(gen.pt()/gen_jet.pt(), 2);
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
      jetstore stored_centraljets({hbbfile::jet::central_jet1, hbbfile::jet::central_jet2, hbbfile::jet::central_jet3},
                                  [](panda::Jet* j) {return j->pt();});
      jetstore stored_cmvas({hbbfile::jet::cmva_jet1, hbbfile::jet::cmva_jet2, hbbfile::jet::cmva_jet3},
                            [](panda::Jet* j) {return j->cmva;});

      // Check if overlaps with EM object
      auto overlap_em = [&em_directions] (panda::Jet& jet) {
        for (auto& dir : em_directions) {
          if (deltaR2(jet.eta(), jet.phi(), dir.first, dir.second) < std::pow(0.4, 2))  // Within dR = 0.4
            return true;
        }
        return false;
      };

      for (auto& jet : event.chsAK4Jets) {

        if (overlap_em(jet) || jet.pt() < 25.0)
          continue;

        // Count jets (including forward)
        auto abseta = std::abs(jet.eta());
        output.set_countjets(hbbfile::countjets::fake, jet, abseta);

        if (not jet.loose)
          continue;

        output.set_countjets(hbbfile::countjets::n, jet, abseta);
        stored_jets.check(jet);

        if (abseta < 2.4) {
          stored_centraljets.check(jet);
          csv_counter.count(jet.csv, output.n_bcsv_loose, output.n_bcsv_medium, output.n_bcsv_tight);
          cmva_counter.count(jet.cmva, output.n_bcmva_loose, output.n_bcmva_medium, output.n_bcmva_tight);
          stored_cmvas.check(jet, &cmva_readers);
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
              const auto& gennu = gen_nu_map.find(gen.get()) != gen_nu_map.end() ? gen_nu_map[gen.get()] : GenNuVec(gen->p4());
              output.set_genjet(jet.branch, *gen, gennu);
            }
          }
        }
      };

      set_jet({&stored_jets, &stored_centraljets});

      // Includes getting secondary vertex and leading leptons
      auto set_bjet = [&output, &set_jet] (std::vector<jetstore*> stores) {
        set_jet(stores);
        for (auto jets : stores) {
          for (auto& jet : jets->store) {
            if (not jet.particle)
              break;

            auto bjet = to_bjet(jet.branch);
            output.set_bvert(bjet, jet.particle->secondaryVertex);

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
                    maxlep = pf.get();    // Dereference the panda::Ref, and then get the address
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

            // jet.extra is the BTagCalibrationReader
            output.set_bjet(bjet, *jet.particle, maxtrkpt, *jet.extra, flavor, nlep, maxlep);
          }
        }
      };

      // set_bjet({&stored_csvs, &stored_cmvas});
      set_bjet({&stored_cmvas});

      if (output.cmva_jet2)
        output.set_hbb(hbbfile::hbb::cmva_hbb, stored_cmvas.store[0].particle->p4() + stored_cmvas.store[1].particle->p4());
      else
        continue;  // Short circuit soft activity this way, because that shit is slow.

      // Soft activity
      auto ellipse = Ellipse(*stored_cmvas.store[0].particle,
                             *stored_cmvas.store[1].particle);

      std::vector<fastjet::PseudoJet> pseudojets;
      pseudojets.reserve(event.pfCandidates.size());
      auto allpseudojets = pseudojets;

      for (auto& cand : event.pfCandidates) {
        auto match_lep = [&cand] (lepstore& leps) {
          for (auto& lep : leps.store) {
            if (lep.particle and lep.particle->matchedPF.get() == &cand)
              return true;
          }
          return false;
        };

        if (cand.vertex.idx() == 0 and cand.track.isValid() and cand.track->highPurity
            and cand.pt() > 0.3 and
            not (std::abs(cand.track->dz()) > 0.2 ||
                 match_lep(stored_muons) || match_lep(stored_eles))
            ) {
          allpseudojets.emplace_back(cand.px(), cand.py(), cand.pz(), cand.e());
          if (not ellipse.inside(cand))
            pseudojets.emplace_back(cand.px(), cand.py(), cand.pz(), cand.e());
        }
      }

      std::vector<std::pair<hbbfile::softcount, std::vector<fastjet::PseudoJet>*>> soft_inputs = {
        {hbbfile::softcount::n_soft, &pseudojets}, {hbbfile::softcount::n_soft_all, &allpseudojets}
      };

      for (auto& jets : soft_inputs) {

        auto sequence = fastjet::ClusterSequence(*(jets.second), {fastjet::JetAlgorithm::antikt_algorithm, 0.4});
        auto softjets = sequence.inclusive_jets(2.0); // Only want pT > 2.0

        for (auto& soft : softjets)
          output.set_softcount(jets.first, soft.pt());
      }

      output.fill(recoilvec);
    }
    input.Close();
  }

  output.write(&all_hist);
  return 0;
}

int main(int argc, char** argv) {
  debug::init();
  return parse_then_send(argc, argv, parsed_main);
}
