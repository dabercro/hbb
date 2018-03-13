#include <cmath>
#include <iostream>

#include "hbbfile.h"

#include "checkrun.h"
#include "feedpanda.h"
#include "btagreaders.h"
#include "misc.h"
#include "debugevent.h"
#include "roccor.h"

#include "fastjet/ClusterSequence.hh"

#include "SkimmingTools/interface/CmsswParse.h"
#include "SkimmingTools/interface/ObjectStore.h"

#include "TH1F.h"

// Terminating case first is an object store, then a list of set functions to call on it
template<typename S, typename F> void set_particles(S& store, F& function) {
  for (auto& particle : store.store) {
    // Just check that each particle exists
    if (not particle.particle)
      break;
    // If it does, call the inner function
    function(particle);
  }
}

// More than two arguments is handled this way...
template<typename S, typename F, typename... Funcs> void set_particles(S& store, F& function, Funcs... funcs) {
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

      // Defined in debugevent.h
      if (debug::debug) {
        static bool processed = false;
        if (processed)
          break;

        if (debug::check(event.runNumber, event.lumiNumber, event.eventNumber)) {
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
          event.chsAK4Jets.dump();
        }
        else 
          continue;
      }

      all_hist.Fill(0.0, output.mc_weight);

      //// FILTER ////
      if (not checkrun(event.runNumber, event.lumiNumber)) {
        if (debug::debug)
          std::cout << "Good run filter: " << event.runNumber << " " << event.lumiNumber << std::endl;
        continue;
      }

      // Check triggers
      for (auto token : met_trigger_tokens) {
        if (event.triggerFired(token)) {
          output.met_trigger = true;
          break;
        }
      }

      //// PHOTONS ////

      std::vector<std::pair<float, float>> em_directions; // Pairs of eta, phi for preselected leptons for cleaning

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

      TLorentzVector lepvec {};
      std::vector<panda::Lepton*> selected_leps;

      // Define default checks here
      // We'll do a lambda with no arguments for lazy evaluation (class LazyId defined in misc.h)

      auto check_lep = [&output, &lepvec, &em_directions, &selected_leps]
        (hbbfile::lep branch, panda::Lepton& lep, float reliso, float corrpt,
         LazyCuts ids) {
        // Definitions straight from AN
        if (ids.presel()) {
          if (debug::debug) {
            std::cout << "Placing lepton for cleaning [pt, eta, phi, m, reliso] = ["
                      << lep.pt() << ", " << lep.eta() << ", " << lep.phi() << ", " << lep.m() << ", "
                      << reliso << "]" << std::endl;
          }
          em_directions.emplace_back(lep.eta(), lep.phi());
          if (ids.loose()) {
            selected_leps.push_back(&lep);
            output.set_lep(branch, lep, {reliso, corrpt}, ids);  // Only want to store leptons that are at least loose
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
        pt = corrpt;
        auto reliso = lep.combIso()/pt;

        if(debug::debug)
          std::cout << "Muon with pt " << lep.pt() << " Corrected to " << corrpt << " has reliso " << reliso << std::endl;

        check_lep(hbbfile::lep::muon, lep, reliso, corrpt,
                  {[&] {   // Muon preselection
                      return pt > 5.0 and abseta < 2.4 and lep.loose and
                        lep.dxy < 0.5 and lep.dz < 1.0 and reliso < 0.4;
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
      for (auto& lep : event.electrons) {

        auto abseta = std::abs(lep.eta());
        auto pt = lep.pt();
        auto corrpt = lep.smearedPt;
        pt = corrpt;
        auto reliso = lep.combIso()/pt;

        if(debug::debug)
          std::cout << "Electron with pt " << lep.pt() << " Corrected to " << corrpt << " has reliso " << reliso << std::endl;

        check_lep(hbbfile::lep::muon, lep, reliso, corrpt,
                  {[&] {   // Electron preselection
                      return lep.smearedPt > 7.0 and abseta < 2.4 and
                        lep.dxy < 0.05 and lep.dz < 0.20 and
                        reliso < 0.4;
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
          if (deltaR2(jet.eta(), jet.phi(), dir.first, dir.second) < std::pow(0.4, 2)) {  // Within dR = 0.4
            if (debug::debug)
              std::cout << "Jet with pt " << jet.pt() << " cleaned" << std::endl;
            return true;
          }
        }
        return false;
      };

      for (auto& jet : event.chsAK4Jets) {

        if (overlap_em(jet) or jet.pt() < 20.0 or not jet.loose)
          continue;


        LazyCuts cmva_cuts = {jet.cmva, -0.5884, 0.4432, 0.9432};
        LazyCuts csv_cuts = {jet.csv, 0.5426, 0.8484, 0.9535};

        // Count jets (including forward)
        auto abseta = std::abs(jet.eta());
        output.set_countjets(jet, abseta, cmva_cuts, csv_cuts);

        stored_jets.check(jet);

        if (abseta < 2.4) {
          stored_centraljets.check(jet);
          stored_cmvas.check(jet, &cmva_readers);
        }
      }

      auto set_jet = [&output] (jetstore::Particle& jet) {
        output.set_jet(jet.branch, *jet.particle);
      };
      set_particles({&stored_jets, &stored_centraljets}, set_jet);

      // Includes getting secondary vertex and leading leptons
      auto set_bjet = [&output, &gen_nu_map] (jetstore::Particle& jet) {
        auto bjet = to_bjet(jet.branch);

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

          const auto& gennu = gen_nu_map.find(gen.get()) != gen_nu_map.end() ? gen_nu_map[gen.get()] : GenNuVec(gen->p4());
          output.set_genjet(bjet, *gen, gennu);
        }

        // jet.extra is the BTagCalibrationReader
        output.set_bjet(bjet, *jet.particle, maxtrkpt, *jet.extra, flavor, nlep, maxlep);

        output.set_bvert(bjet, jet.particle->secondaryVertex);
      };

      set_particles(stored_cmvas, set_jet, set_bjet);

      if (output.cmva_jet2)
        output.set_hbb(hbbfile::hbb::cmva_hbb);
      else {
        if (debug::debug)
          std::cout << "No second jet. Jet 1 pT: " << output.cmva_jet1_pt << std::endl;
        continue;  // Short circuit soft activity this way, because that shit is slow.
      }

      // Soft activity
      auto ellipse = Ellipse(*stored_cmvas.store[0].particle,
                             *stored_cmvas.store[1].particle);

      std::vector<fastjet::PseudoJet> pseudojets;
      pseudojets.reserve(event.pfCandidates.size());
      auto allpseudojets = pseudojets;

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

      output.fill(event.pfMet.v() + lepvec.Vect().XYvector());
    }
    input.Close();
  }

  output.write(&all_hist);
  return 0;
}

int main(int argc, char** argv) {
  return parse_then_send(argc, argv, parsed_main);
}
