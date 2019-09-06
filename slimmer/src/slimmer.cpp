#include <cmath>
#include <iostream>
#include <memory>

#include "hbbfile.h"
#include "triggers.h"
#include "jetselect.h"
#include "leptonselect.h"
#include "softcalc.h"
#include "feedpanda.h"
#include "myrandom.h"
#include "checkrun.h"

#include "fastjet/ClusterSequence.hh"

#include "crombie/ObjectStore.h"
#include "crombie/CmsswParse.h"

#include "TH1F.h"
#include "TH1D.h"

using namespace crombie;

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

  jetselect::JetSelector jetselector {};

  // Loop over all input files
  for (int i_file = 1; i_file < argc - 1; i_file++) {

    std::cout << "Running over file " << argv[i_file]
              << " (" << i_file << "/" << (argc - 2) << ")" << std::endl;

    // Get the PandaTree
    TFile input {argv[i_file]};
    auto* events_tree = static_cast<TTree*>(input.Get("events"));
    panda::Event event;
    feedpanda(event, events_tree);
    auto nentries = input::maxevents ? input::maxevents : events_tree->GetEntries();

    triggers::init(event);
    jetselector.init(event);

    // Get the hSumW
    auto* sumW = static_cast<TH1D*>(input.Get("hSumW"));
    if (sums.GetNbinsX() == 1)  // SumW has 8 bins, but default constructor makes 1
      sums = *sumW;
    else
      sums.Add(sumW);

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

      if (not checkrun(event.runNumber,
                       event.lumiNumber))
        continue;

      output.reset(event);

      // Defined in debugevent.h
      if (debugevent::debugevent) {
        static bool processed = false;
        if (processed)
          break;

        if (debugevent::check(event.runNumber,
                              event.lumiNumber,
                              event.eventNumber)) {
          processed = true;
          std::cout << std::endl << "Found Event in row " << entry << std::endl << std::endl;
          event.pfMet.dump();
          event.photons.dump();
          event.muons.dump();
          event.electrons.dump();
          event.ak4GenJets.dump();
          // (event.*ak4jets).dump();
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

      // Set the PFCandidate map used for redoing reliso calculations
      pfcands::pfmap.AddParticles(event.pfCandidates);
      // Gen Jets for determining flavor
      if (not event.isData)
        btag::genjetmap.AddParticles(event.ak4GenJets);

      //// PHOTONS ////
      if (debugevent::debug)
        std::cout << "Starting photons" << std::endl;

      for (auto& pho : event.photons) {
        output.n_pho_loose += pho.loose;
        output.n_pho_medium += pho.medium;
        output.n_pho_tight += pho.tight;
      }

      //// LEPTONS ////

      leptonselect::SelectedLeptons selectedleptons {event};

      output.n_tau_loose = selectedleptons.numloosetau;

      TLorentzVector lepvec {};


      using lepstore = ObjectStore::ObjectStore<hbbfile::dilep, panda::Lepton>;

      lepstore stored_muons({hbbfile::dilep::muon1, hbbfile::dilep::muon2},
                            [] (const panda::Lepton& l) { return l.pt(); });

      lepstore stored_eles({hbbfile::dilep::ele1, hbbfile::dilep::ele2},
                           [] (const panda::Lepton& l) { return l.pt(); });

      output.set_countselected(selectedleptons);

      for (auto& info : selectedleptons.loose)
        (info.ismuon ? stored_muons : stored_eles).check(*(info.particle));

      for (auto& info : selectedleptons.tight) {
        auto& lep = *info.particle;
        lepvec += lep.p4();                         // Only want to add the one tight lepton for recoil in ttbar
        output.tight_lep_pt = std::max(output.tight_lep_pt,
                                       static_cast<decltype(output.tight_lep_pt)>(lep.pt()));
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

      //// GEN BOSON FOR KFACTORS AND TTBAR FOR PT SCALING ////
      if (debugevent::debug)
        std::cout << "Starting gen bosons, size: " << event.genParticles.size() << std::endl;

      for (auto& gen : event.genParticles) {
        auto abspdgid = std::abs(gen.pdgid);
        bool is_neutrino = (abspdgid == 12 or abspdgid == 14 or abspdgid == 16);
        if (input::version >= 12 and gen.miniaodPacked and not is_neutrino)
          continue;
        if (not output.genboson and (abspdgid == 23 or abspdgid == 24))
          output.set_gen(hbbfile::gen::genboson, gen);

        else if (not output.gen_t and gen.pdgid == 6)
          output.set_gen(hbbfile::gen::gen_t, gen);

        else if (not output.gen_tbar and gen.pdgid == -6)
          output.set_gen(hbbfile::gen::gen_tbar, gen);

        else if (abspdgid == 25)
          gen_higgs.check(gen);

        else if (is_neutrino and (input::version < 12 or gen.miniaodPacked)) {
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

      auto updated_jets = jetselector.update_event();
      output.pfmet = updated_jets.pfmet.pt;
      output.pfmet_phi = updated_jets.pfmet.phi;

      // We want the two jets with the highest CMVA
      using jetstore = ObjectStore::ObjectStore<hbbfile::bjet, panda::Jet, struct lazy::Evaled>;

      // jetstore stored_bjets({hbbfile::bjet::jet1, hbbfile::bjet::jet2, hbbfile::bjet::jet3},
      jetstore stored_bjets({hbbfile::bjet::jet1, hbbfile::bjet::jet2},
                            input::tagger == input::btagger::cmva ?
                            [] (const panda::Jet& j) { return j.cmva + 2; } :
                            [] (const panda::Jet& j) { return j.deepCSVb + j.deepCSVbb + 2; });

      for (auto& jet : updated_jets.ak4jets) {

        if (jet.pt() < 15.0 or not jetselect::clean_jet(jet, selectedleptons)) {
          if (debugevent::debug)
            std::cout << "Jet with pt " << jet.pt() << " did not pass initial jet filter" << std::endl;
          continue;
        }

        output.set_countnopuid(jet);

        if (not puid::loose(jet))
          continue;

        lazy::LazyCuts cmva_cuts = {jet.cmva, -0.5884, 0.4432, 0.9432};
        lazy::LazyCuts csv_cuts = {jet.csv, 0.5426, 0.8484, 0.9535};
        lazy::LazyCuts deepCSV_cuts = {jet.deepCSVb + jet.deepCSVbb, 0.1241, 0.4184, 0.7527};

        // Count jets (including forward)
        auto abseta = std::abs(jet.eta());
        output.set_countjets(jet, abseta, cmva_cuts, csv_cuts, deepCSV_cuts);

        if (abseta < 2.5 and jet.loose) {
          stored_bjets.check(jet, {input::tagger == input::btagger::cmva ? cmva_cuts : deepCSV_cuts});
          output.set_bsf(jet, (input::tagger == input::btagger::cmva ? cmva_cuts : deepCSV_cuts).loose());
        }
      }

      set_particles(stored_bjets,
                    [&output] (const jetstore::Particle& jet) {
                      output.set_bjet(jet.branch, *jet.particle, jet.extra);
                    });

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
        auto match_lep = [&cand, &selectedleptons] () {
          for (auto& info : selectedleptons.loose) {
            if (info.particle->matchedPF.get() == &cand)
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

      output.fill(event, updated_jets.pfmet.v() + lepvec.Vect().XYvector());
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
