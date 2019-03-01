#include <cmath>
#include <iostream>

#include "hbbfile.h"
#include "misc.h"

#include "fastjet/ClusterSequence.hh"

#include "crombie/CmsswParse.h"
#include "crombie/ObjectStore.h"

#include "TH1F.h"
#include "TH1D.h"

using namespace crombie;

// Need to implement this here due to forward declaration in puid.h
void puid::update (const panda::Jet& jet, hbbfile* output) {
  output->set_countnopuid(jet);
}

// Terminating case first is an object store, then a list of set functions to call on it
template<typename S, typename F> void set_particles(S& store, const F& function) {
  for (auto& particle : store.store) {
    // Just check that each particle exists
    if (not particle.particle)
      break;
    // If it does, call the inner function
    function(particle);
  }
}

// More than two arguments is handled this way...
template<typename S, typename F, typename... Funcs> void set_particles(S& store, const F& function, Funcs... funcs) {
  set_particles(store, function);
  set_particles(store, funcs...);
}

template<typename S, typename... Funcs> void set_particles(std::initializer_list<S*>&& stores, Funcs... funcs) {
  for (auto* store : stores)
    set_particles(*store, funcs...);
}

int parsed_main(int argc, char** argv) {

  hbbfile output {argv[argc - 1]};
  TH1F all_hist {"htotal", "htotal", 1, -1, 1};
  TH1D sums;

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

    // Get the hSumW
    auto* sumW = static_cast<TH1D*>(input.Get("hSumW"));
    if (sums.GetNbinsX() == 1)  // SumW has 8 bins, but default constructor makes 1
      sums = *sumW;
    else
      sums.Add(sumW);

    //// TRIGGERS ////

    using tokens = std::vector<unsigned>;

    auto get_tokens = [&event] (const std::vector<const char*>& paths) {
      tokens output;
      for (auto path : paths)
        output.push_back(event.registerTrigger(path));
      return output;
    };

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
      "HLT_PFMETNoMu100_PFMHTNoMu100_IDTight_PFHT60",
      "HLT_PFMETNoMu100_PFMHTNoMu100_IDTight",
      "HLT_PFMETNoMu110_PFMHTNoMu110_IDTight",
      "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight",
      "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60",
      "HLT_PFMETNoMu130_PFMHTNoMu130_IDTight",
      "HLT_PFMETNoMu140_PFMHTNoMu140_IDTight"
    };

    tokens met_trigger_tokens = get_tokens(met_trigger_paths);

    const std::vector<const char*> hbb_2016_paths = {
      "HLT_PFMETNoMu110_PFMHTNoMu110_IDTight",
      "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight",
      "HLT_PFMET170_NoiseCleaned",
      "HLT_PFMET170_HBHECleaned",
      "HLT_PFMET170_HBHE_BeamHaloCleaned"
    };

    tokens hbb_2016_tokens = get_tokens(hbb_2016_paths);

    const std::vector<const char*> overlap_paths = {
      "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"
    };

    tokens overlap_tokens = get_tokens(overlap_paths);

    // Loop over tree
    for(decltype(nentries) entry = 0; entry != nentries; ++entry) {
      if (entry % 10000 == 0) {
        std::cout << "Processing events: ... " << float(entry)/nentries*100 << " %" << std::endl;
        // Set the global generator seed to the first event number of a file
        if (not entry)
          myrandom::gen.SetSeed(event.eventNumber);
      }

      event.getEntry(*events_tree, entry);
      all_hist.Fill(0.0, event.weight);

      // Should check good lumi towards the beginning

      if (not checkrun(event.runNumber, event.lumiNumber))
        continue;

      output.reset(event);

      // Defined in debugevent.h
      if (debugevent::debugevent) {
        static bool processed = false;
        if (processed)
          break;

        if (debugevent::check(event.runNumber, event.lumiNumber, event.eventNumber)) {
          processed = true;
          std::cout << std::endl << "Found Event in row " << entry << std::endl << std::endl;
          for (auto token : met_trigger_tokens) {
            if (event.triggerFired(token))
              std::cout << met_trigger_paths[token] << std::endl;
          }
          event.pfMet.dump();
          event.photons.dump();
          event.muons.dump();
          event.electrons.dump();
          event.ak4GenJets.dump();
          event.chsAK4Jets.dump();
          int icand = 0;
          for (auto& cand : event.pfCandidates)
            std::cout << "PF Candidate: " << icand++
                      << "; pt: " << cand.pt()
                      << "; eta: " << cand.eta()
                      << "; phi: " << cand.phi() << std::endl;
        }
        else
          continue;
      }

      //// TRIGGERS ////
      auto check_tokens = [&event] (tokens& trigger_tokens) {
        for (auto token : trigger_tokens) {
          if (event.triggerFired(token))
            return true;
        }
        return false;
      };

      output.met_trigger = check_tokens(met_trigger_tokens);
      output.hbb_2016_trigger = check_tokens(hbb_2016_tokens);
      output.overlap_trigger = check_tokens(overlap_tokens);

      // Set the PFCandidate map used for redoing reliso calculations
      pfcands::pfmap.AddParticles(event.pfCandidates);
      // Gen Jets for determining flavor
      if (not event.isData)
        btag::genjetmap.AddParticles(event.ak4GenJets);

      //// PHOTONS ////
      if (debugevent::debug)
        std::cout << "Starting photons" << std::endl;

      std::vector<std::pair<float, float>> em_directions; // Pairs of eta, phi for preselected leptons for cleaning

      for (auto& pho : event.photons) {
        output.n_pho_loose += pho.loose;
        output.n_pho_medium += pho.medium;
        output.n_pho_tight += pho.tight;
      }

      //// TAUS ////
      if (debugevent::debug)
        std::cout << "Starting taus" << std::endl;
      for (auto& tau : event.taus) {
        if (tau.pt() > 18 && std::abs(tau.eta()) < 2.3 && tau.decayMode && tau.isoDeltaBetaCorr)
          output.n_tau_loose++;
      }

      //// OTHER LEPTONS ////
      if (debugevent::debug)
        std::cout << "Starting other leptons" << std::endl;

      TLorentzVector lepvec {};
      std::vector<panda::Lepton*> selected_leps;

      // Define default checks here
      // We'll do a lambda with no arguments for lazy evaluation

      using lepstore = ObjectStore::ObjectStore<hbbfile::dilep, panda::Lepton>;

      lepstore stored_muons({hbbfile::dilep::muon1, hbbfile::dilep::muon2},
                            [] (const panda::Lepton& l) { return l.pt(); });

      lepstore stored_eles({hbbfile::dilep::ele1, hbbfile::dilep::ele2},
                           [] (const panda::Lepton& l) { return l.pt(); });


      auto set_lep = [&output, &lepvec, &em_directions, &selected_leps,
                      &stored_muons, &stored_eles]
        (hbbfile::lep branch, panda::Lepton& lep, LepInfo info,
         lazy::LazyCuts ids) {
        // Definitions straight from AN
        if (ids.presel()) {
          if (debugevent::debug) {
            std::cout << "Placing lepton for cleaning [pt, eta, phi, m, reliso] = ["
                      << lep.pt() << ", " << lep.eta() << ", " << lep.phi() << ", " << lep.m() << ", "
                      << info.reliso << "]" << std::endl;
          }
          if (info.reliso < 0.4)
            em_directions.emplace_back(lep.eta(), lep.phi());
          output.set_lep(branch, lep, info, ids);
          if (ids.loose()) {
            selected_leps.push_back(&lep);

            (branch == hbbfile::lep::muon ? stored_muons : stored_eles).check(lep);

            if (ids.medium() && ids.tight()) {
              lepvec += lep.p4();                         // Only want to add the one tight lepton for recoil in ttbar
              output.tight_lep_pt = std::max(output.tight_lep_pt,
                                             static_cast<decltype(output.tight_lep_pt)>(lep.pt()));
            }
          }
        }
      };

      // Loop over muons
      for (auto& lep : event.muons) {

        auto abseta = std::abs(lep.eta());
        auto pt = lep.pt();
        auto corrpt = pt * roccor::scale(event, lep); // This scale() function must be called for every muon in the event for repeatable random numbers
        auto reliso = lep.combIso()/pt;
        auto minireliso = reliso::minireliso(lep, event.rho);

        if(debugevent::debug)
          std::cout << "Muon with pt " << lep.pt() << " Corrected to " << corrpt
                    << " has reliso " << reliso << " minireliso " << minireliso << std::endl;

        set_lep(hbbfile::lep::muon, lep, {reliso, minireliso, corrpt},
                {[&] {   // Muon preselection
                    return pt > 5.0 and abseta < 2.4 and lep.loose and
                      lep.dxy < 0.5 and lep.dz < 1.0 and std::min(reliso, minireliso) < 0.4;
                  },
                  [&] {   // Loose muons
                    return lep.pf and (lep.global or lep.tracker);
                  },
                  [&] {   // Medium muons; they don't seem to do anything with those
                    return true;
                  },
                  [&] {   // Tight muons; there's probably also a pT cut, but they don't give it directly
                    return lep.tight and reliso < 0.15 and
                      lep.global and lep.normChi2 < 10.0 and
                      lep.nValidMuon > 0 and lep.nMatched > 1 and
                      lep.nValidPixel > 0 and lep.trkLayersWithMmt > 5 and
                      lep.dxy < 0.2 and lep.dz < 0.5;
                  }
                });
      }

      // Loop over electrons

      // Make a map to go from PFCands to Electrons for checking

      std::map<const panda::Ref<panda::PFCand>::index_type, const panda::Electron*> pf_to_electron;

      for (auto& lep : event.electrons) {

        const auto pf = lep.matchedPF;
        if (pf.isValid())
          pf_to_electron[pf.idx()] = &lep;

        auto abseta = std::abs(lep.eta());
        auto pt = lep.pt();
        auto corrpt = lep.smearedPt;
        auto reliso = lep.combIso()/pt;
        auto minireliso = reliso::minireliso(lep);

        if(debugevent::debug)
          std::cout << "Electron with pt " << lep.pt() << " Corrected to " << corrpt
                    << " has reliso " << reliso << " minireliso " << minireliso << std::endl;

        set_lep(hbbfile::lep::ele, lep, {reliso, minireliso, corrpt},
                {[&] {   // Electron preselection
                    return pt > 7.0 and abseta < 2.4 and
                      lep.dxy < 0.05 and lep.dz < 0.20 and std::min(reliso, minireliso) < 0.4;
                  },
                  [&] {   // Loose electrons for conservative event classification
                    return lep.mvaWP90;
                  },
                  [&] {   // "Medium" electrons, which I define as the cut applied to match MVA training
                    return pt > 15.0 and lep.hOverE < 0.09 and lep.trackIso/pt < 0.18 and
                      ((abseta < 1.4442 and lep.sieie < 0.012 and
                        lep.ecalIso/pt < 0.4 and lep.hcalIso/pt < 0.25 and
                        std::abs(lep.dEtaInSeed) < 0.0095 and std::abs(lep.dPhiIn) < 0.065) or
                       (abseta > 1.5660 and lep.sieie < 0.033 and
                        lep.ecalIso/pt < 0.45 and lep.hcalIso/pt < 0.28)
                       );
                  },
                  [&] {   // Tight electrons
                    return lep.mvaWP80;
                  }
                });
      }

      set_particles({&stored_muons, &stored_eles},
                    [&output](lepstore::Particle& entry) {
                      output.set_dilep(entry.branch, *entry.particle);
                    });

      //// GEN PARTICLES ////
      if (debugevent::debug)
        std::cout << "Starting gen particles" << std::endl;

      for (auto& gen_jet : event.ak4GenJets) {
        if (gen_jet.pt() > 20 && std::abs(gen_jet.eta()) < 2.4 && (gen_jet.numB || std::abs(gen_jet.pdgid) == 5))
          ++output.n_genB;
      }

      if (debugevent::debug)
        std::cout << "  counted b jets" << std::endl;

      using genhstore = ObjectStore::ObjectStore<hbbfile::higgs, panda::GenParticle>;

      // Get the generator particles that are closest to the reconstructed Higgs

      genhstore gen_higgs {
        {hbbfile::higgs::hbb},
        [&output] (const panda::GenParticle& gen) {return deltaR2(output.hbb_eta, output.hbb_phi, gen.eta(), gen.phi());},
        genhstore::order::eAsc
      };

      pfcands::MakeNuJets(event);

      std::map<const panda::GenJet*, GenNuVec> gen_nu_map;

      //// GEN BOSON FOR KFACTORS AND TTBAR FOR PT SCALING ////
      if (debugevent::debug)
        std::cout << "Starting gen bosons, size: " << event.genParticles.size() << std::endl;

      for (auto& gen : event.genParticles) {
        if (input::version >= 12 and gen.miniaodPacked)
          continue;
        auto abspdgid = std::abs(gen.pdgid);
        if (not output.genboson and (abspdgid == 23 or abspdgid == 24))
          output.set_gen(hbbfile::gen::genboson, gen);

        else if (not output.gen_t and gen.pdgid == 6)
          output.set_gen(hbbfile::gen::gen_t, gen);

        else if (not output.gen_tbar and gen.pdgid == -6)
          output.set_gen(hbbfile::gen::gen_tbar, gen);

        else if (abspdgid == 25)
          gen_higgs.check(gen);

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
        else if (not (gen.parent.isValid() and gen.parent->pdgid == gen.pdgid)) { // Count bs for stitching
          if (abspdgid == 5 and gen.testFlag(panda::GenParticle::kIsPrompt))
            ++output.nB;
          auto mod10k = abspdgid % 10000;
          if (gen.testFlag(panda::GenParticle::kIsDecayedLeptonHadron) and
              ((mod10k >= 500 and mod10k < 600) or (mod10k >= 5000 and mod10k < 6000)))
            ++output.stat2b;
        }
      }

      if (debugevent::debug)
        std::cout << "  Finished looping all particles" << std::endl;

      set_particles(gen_higgs,
                    [&output](genhstore::Particle& entry) {
                      output.set_hbbgen(entry.branch, *entry.particle, sqrt(entry.result));
                    });

      //// JETS ////
      if (debugevent::debug)
        std::cout << "Starting jets" << std::endl;

      // We want the two jets with the highest CMVA
      using jetstore = ObjectStore::ObjectStore<hbbfile::bjet, panda::Jet, struct lazy::Evaled>;

      // jetstore stored_bjets({hbbfile::bjet::jet1, hbbfile::bjet::jet2, hbbfile::bjet::jet3},
      jetstore stored_bjets({hbbfile::bjet::jet1, hbbfile::bjet::jet2},
                            input::tagger == input::btagger::cmva ?
                            [] (const panda::Jet& j) { return j.cmva + 2; } :
                            [] (const panda::Jet& j) { return j.deepCSVb + 2; });

      // Check if overlaps with EM object
      auto overlap_em = [&em_directions] (panda::Particle& jet, double dr2) {
        for (auto& dir : em_directions) {
          if (deltaR2(jet.eta(), jet.phi(), dir.first, dir.second) < dr2) {
            if (debugevent::debug)
              std::cout << "Jet with pt " << jet.pt() << " cleaned" << std::endl;
            return true;
          }
        }
        return false;
      };

      for (auto& jet : event.chsAK4Jets) {

        if (overlap_em(jet, std::pow(0.4, 2)) or jet.pt() < 20.0 or not puid::loose(jet, &output)) {
          if (debugevent::debug)
            std::cout << "Jet with pt " << jet.pt() << " did not pass initial jet filter" << std::endl;
          continue;
        }

        lazy::LazyCuts cmva_cuts = {jet.cmva, -0.5884, 0.4432, 0.9432};
        lazy::LazyCuts csv_cuts = {jet.csv, 0.5426, 0.8484, 0.9535};
        lazy::LazyCuts deepCSV_cuts = {jet.deepCSVb, 0.1522, 0.4941, 0.8001};

        // Count jets (including forward)
        auto abseta = std::abs(jet.eta());
        output.set_countjets(jet, abseta, cmva_cuts, csv_cuts, deepCSV_cuts);

        if (abseta < 2.4 and jet.loose) {
          stored_bjets.check(jet, {input::tagger == input::btagger::cmva ? cmva_cuts : deepCSV_cuts});
          output.set_bsf(jet, (input::tagger == input::btagger::cmva ? cmva_cuts : deepCSV_cuts).loose());
        }
      }

      // Includes getting secondary vertex and leading leptons
      auto set_bjet = [&output, &gen_nu_map, &pf_to_electron, &event] (const jetstore::Particle& jet) {
        auto bjet = jet.branch;

        int nlep = 0;
        const panda::Lepton* maxlep = nullptr;

        decltype(maxlep->pt()) maxtrkpt = 0;
        decltype(maxlep->pt()) maxpfpt = 0;

        if (debugevent::debugevent)
          std::cout << "For jet with pt " << jet.particle->pt() << std::endl;

        // for (auto pf : pfcands::pfmap(jet.particle->eta(), jet.particle->phi(), 0.4)) {
        for (auto pf : jet.particle->constituents) {
          if (debugevent::debugevent)
            std::cout << "PF Check: " << pf.isValid() << " " << pf.idx() << " " << pf->pt() << " " << pf->q() << std::endl;

          if (pf.isValid()) {
            auto pt = pf->pt();
            maxpfpt = std::max(maxpfpt, pt);
            if (pf->q())
              maxtrkpt = std::max(maxtrkpt, pt);
            // auto pdgid = abs(pf->pdgId());
            // if (pdgid == 13 || ((pf_to_electron.find(pf.idx()) != pf_to_electron.end()) and
            //                     electronid::is_good(*pf_to_electron[pf.idx()], *jet.particle))) {
            //   nlep++;
            //   if (not maxlep or pt > maxlep->pt())
            //     maxlep = pf.get();
            // }
          }
          else
            std::cout << "BAD PF: " << event.eventNumber << " " << pf.idx() << std::endl;
        }

        auto check_lep = [&] (const panda::Lepton& lep, std::function<bool()> sel) {
          if (sel() and deltaR2(jet.particle->eta(), jet.particle->phi(), lep.eta(), lep.phi()) < 0.16) {
            nlep++;
            if (not maxlep or lep.pt() > maxlep->pt())
              maxlep = &lep;
          }
        };

        for (auto& ele : event.electrons)
          check_lep(ele,
                    [&ele] () {
                      return ele.pt() >= 5 and std::abs(ele.eta()) <= 2.5 and std::abs(ele.dxy) <= 0.5 and std::abs(ele.dz) <= 1.0 and ele.nMissingHits <= 1;
                    });

        for (auto& mu : event.muons)
          check_lep(mu,
                    [&mu] () {
                      return mu.pt() >= 3 and std::abs(mu.eta()) <= 2.4 and std::abs(mu.dxy) <= 0.5 and std::abs(mu.dz) <= 1.0;
                    });

        // Determine the flavor of the jet
        auto& gen = jet.particle->matchedGenJet;
        if (gen.isValid()) {
          const auto& gennu = gen_nu_map.find(gen.get()) != gen_nu_map.end() ? gen_nu_map[gen.get()] : GenNuVec(gen->p4());
          output.set_genjet(bjet, *gen, gennu);
        }

        output.set_bjet(bjet, *jet.particle, maxtrkpt, maxpfpt, nlep, maxlep, jet.extra);
        output.set_bvert(bjet, jet.particle->secondaryVertex);
        output.set_reclustered(bjet, pfcands::NuJet(*jet.particle));
      };

      set_particles(stored_bjets, set_bjet);

      //// FAT JETS ////
      if (debugevent::debug)
        std::cout << "Starting fat jets" << std::endl;

      using fatstore = ObjectStore::ObjectStore<hbbfile::fatjet, panda::FatJet>;

      auto fill_fat_jet = [&output, &overlap_em] (const std::vector<hbbfile::fatjet>& branches, panda::FatJetCollection& fatjets) {
        fatstore stored_fat(branches, [] (const panda::FatJet& j) {return j.pt();});

        for (auto& fatjet : fatjets) {
          if (overlap_em(fatjet, std::pow(0.8, 2)) or not fatjet.monojet or not fatjet.loose or std::abs(fatjet.eta()) > 2.4)
            continue;
          output.set_countfat(fatjet);
          stored_fat.check(fatjet);
        }

        auto set_fatjet = [&output] (fatstore::Particle& fat) {
          using counter = decltype(output.n_alljet);

          // max: The number of indices to loop over
          // is_iso: A function that takes the index and returns true if this should count as an iso jet
          // Returns: Number of isolated jets that pass is_iso function
          auto count = [] (counter max, std::function<bool(counter)>&& is_iso) {
            counter n = 0;
            for (counter i_jet = 0; i_jet < max; ++i_jet)
              n += (is_iso(i_jet));

            return n;
          };

          using offset_target = decltype(output.jet_eta);

          // Use this to generate a function that says when a jet is isolated from the fat jet
          auto fat_iso = [&fat, &output] (offset_target hbbfile::*eta_offset, offset_target hbbfile::*phi_offset) {
            return [&fat, &output, eta_offset, phi_offset] (counter i_jet) {
              return (deltaR2(fat.particle->eta(), fat.particle->phi(),
                              (output.*eta_offset)[i_jet],
                              (output.*phi_offset)[i_jet]) > std::pow(0.8, 2));
            };
          };

          // Number of isolated jets
          counter n_iso = count(output.n_alljet,
                                fat_iso(&hbbfile::jet_eta, &hbbfile::jet_phi));

          auto b_overlapper = fat_iso(&hbbfile::bjet_eta, &hbbfile::bjet_phi);
          // Number of isolated (loose) bjets
          counter n_iso_b = count(output.n_allbjet,
                                  [&output, &b_overlapper] (counter i_jet) {
                                    return (output.bjet_isloose[i_jet] and
                                            b_overlapper(i_jet));
                                  });

          output.set_fatjet(fat.branch, *fat.particle, n_iso, n_iso_b);
        };
        set_particles(stored_fat, set_fatjet);
      };
      std::vector<std::pair<std::vector<hbbfile::fatjet>, panda::FatJetCollection>> fatcollections = {
        {{hbbfile::fatjet::ak8fatjet1}, event.puppiAK8Jets},
      };
      for (auto& fats : fatcollections)
        fill_fat_jet(fats.first, fats.second);

      //// HIGGS ////
      if (debugevent::debug)
        std::cout << "Starting Higgs" << std::endl;

      if (output.jet2)
        output.set_higgs(hbbfile::higgs::hbb);

      // if (not debugevent::debug and
      //     not ((output.hbb and output.hbb_pt > 70 and output.hbb_m < 600) or
      //          output.ak8fatjet1_good))
      //   continue;  // Short circuit soft activity this way, because that shit is slow.

      // Soft activity
      const auto ellipse = softcalc::Ellipse(stored_bjets.store[0].particle,
                                             stored_bjets.store[1].particle,
                                             0.5);
      auto dylans = stored_bjets.store[1].particle ?
        DylanSoft(stored_bjets.store[0].particle->eta(), stored_bjets.store[0].particle->phi(),
                  stored_bjets.store[1].particle->eta(), stored_bjets.store[1].particle->phi()) :
        DylanSoft(0, 0, 0, 0);


      std::vector<fastjet::PseudoJet> pseudojets;
      pseudojets.reserve(event.pfCandidates.size());
      auto allpseudojets = pseudojets;
      auto dylanjets = pseudojets;

      for (auto& cand : event.pfCandidates) {
        auto match_lep = [&cand, &selected_leps] () {
          for (auto* lep : selected_leps) {
            if (lep->matchedPF.get() == &cand)
              return true;
          }
          return false;
        };

        if (cand.vertex.idx() == 0 and cand.track.isValid() and cand.track->highPurity
            and cand.pt() > 0.3 and
            not (std::abs(cand.track->dz()) > 0.2 or match_lep())) {

          allpseudojets.emplace_back(cand.px(), cand.py(), cand.pz(), cand.e());
          if (not ellipse.inside(cand))
            pseudojets.emplace_back(cand.px(), cand.py(), cand.pz(), cand.e());
          if (dylans.ellipse_eq(cand.eta(), cand.phi()) > 1.0)
            dylanjets.emplace_back(cand.px(), cand.py(), cand.pz(), cand.e());
        }
      }

      std::vector<std::pair<hbbfile::softcount, std::vector<fastjet::PseudoJet>*>> soft_inputs = {
        {hbbfile::softcount::n_soft, &pseudojets},
        {hbbfile::softcount::n_soft_all, &allpseudojets},
        {hbbfile::softcount::n_soft_dylan, &dylanjets}
      };

      for (auto& jets : soft_inputs) {

        auto sequence = fastjet::ClusterSequence(*(jets.second), {fastjet::JetAlgorithm::antikt_algorithm, 0.4});
        auto softjets = sequence.inclusive_jets(1.0); // Only want pT > 2.0

        for (auto& soft : softjets)
          if (std::abs(soft.eta()) <= 4.7)
            output.set_softcount(jets.first, soft.pt());
      }

      output.fill(event, event.pfMet.v() + lepvec.Vect().XYvector());
    }
    input.Close();
  }

  output.write(&all_hist);
  output.write(&sums);
  return 0;
}

int main(int argc, char** argv) {
  return parse_then_send(argc, argv, parsed_main);
}
