#include <algorithm>
#include <iostream>
#include <utility>
#include <functional>

#include "hbbfile.h"
#include "feedpanda.h"

#include "SkimmingTools/interface/CmsswParse.h"
#include "SkimmingTools/interface/StoreParticles.h"
#include "PandaTree/Objects/interface/Event.h"

#include "TH1F.h"

template<typename T, typename F>
void check_top_two(T& check, std::pair<T*, T*>& top, F get_tag) {
  auto tag = get_tag(&check);
  if (not top.first or tag > get_tag(top.first)) {
    top.second = top.first;
    top.first = &check;
  }
  else if (not top.second or tag > get_tag(top.second))
    top.second = &check;
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
      "HLT_PFMET170_HBHE_BeamHaloCleaned"
    };

    std::vector<unsigned> met_trigger_tokens;

    for (auto path : met_trigger_paths)
      met_trigger_tokens.push_back(event.registerTrigger(path));

    // Loop over tree
    for(decltype(nentries) entry = 0; entry != nentries; ++entry) {
      if (entry % 1000 == 0)
        std::cout << "Processing events: ... " << float(entry)/nentries*100 << " %" << std::endl;
      event.getEntry(*events_tree, entry);
      output.reset(event);

      all_hist.Fill(0.0, (event.weight < 0) ? -1.0 : 1.0);

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

      using lepstore = ObjectStore<hbbfile::lep_enum, panda::Lepton, float>;
      lepstore stored_muons({hbbfile::lep_enum::muon1, hbbfile::lep_enum::muon2}, [](panda::Lepton* l) {return l->pt();});
      lepstore stored_eles({hbbfile::lep_enum::ele1, hbbfile::lep_enum::ele2}, [](panda::Lepton* l) {return l->pt();});

      // Define default checks here
      auto check_lep = [&output, &lepvec] (lepstore& store, panda::Lepton& lep) {
        if (lep.loose) {
          lepvec += lep.p4();
          output.n_looselep++;
          store.check(lep);
          if (lep.medium) {
            output.n_mediumlep++;
            if (lep.tight)
              output.n_tightlep++;
          }
        }
      };

      // Loop over muons
      for (auto& lep : event.muons)
        check_lep(stored_muons, lep);

      // Loop over electrons
      for (auto& lep : event.electrons)
        check_lep(stored_eles, lep);

      auto set_lep = [&output] (lepstore& leps) {
        for (auto& lep : leps.store) {
          if (not lep.second)
            break;
          output.set_lep(lep.first, *lep.second);
        }
      };

      set_lep(stored_muons);
      set_lep(stored_eles);

      //// JETS ////

      // We want the two jets with the highest CSV and CMVA
      using jetstore = ObjectStore<hbbfile::jet_enum, panda::Jet, float>;

      jetstore stored_jets({hbbfile::jet_enum::jet1, hbbfile::jet_enum::jet2},
                           [](panda::Jet* j) {return j->pt();});
      jetstore stored_csvs({hbbfile::jet_enum::csv_jet1, hbbfile::jet_enum::csv_jet2},
                           [](panda::Jet* j) {return j->csv;});
      jetstore stored_cmvas({hbbfile::jet_enum::cmva_jet1, hbbfile::jet_enum::cmva_jet2},
                            [](panda::Jet* j) {return j->cmva;});

      for (auto& jet : event.chsAK4Jets) {

        if (jet.pt() < 20.0)
          continue;

        // Count all jets (including forward)
        output.n_alljet++;
        if (fabs(jet.eta()) > 2.5)
          continue;

        output.n_jet++;
        if (jet.pt() > 30.0)
          output.n_hardjet++;

        stored_jets.check(jet);
        stored_csvs.check(jet);
        stored_cmvas.check(jet);
      }

      auto set_jet = [&output] (jetstore& jets) {
        for (auto& jet : jets.store) {
          if (not jet.second)
            break;
          output.set_jet(jet.first, *jet.second);
          auto& gen = jet.second->matchedGenJet;
          if (gen.isValid())
            output.set_genjet(jet.first, *gen);
        }
      };

      set_jet(stored_jets);

      // Includes getting secondary vertex and leading leptons
      auto set_bjet = [&output, &set_jet] (jetstore jets) {
        set_jet(jets);
        for (auto& jet : jets.store) {
          if (not jet.second)
            break;

          auto bjet = to_bjet(jet.first);
          auto& vert = jet.second->secondaryVertex;
          if (vert.isValid())
            output.set_bjet(bjet, *vert);

          int nlep = 0;
          const panda::PFCand* maxlep = nullptr;

          decltype(maxlep->pt()) maxtrkpt = 0;

          for (auto pf : jet.second->constituents) {
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
            output.set_bleps(bjet, *jet.second, nlep, *maxlep);
        }
      };

      set_bjet(stored_csvs);
      set_bjet(stored_cmvas);

      if (stored_csvs.store[1].second)
        output.set_hbb(hbbfile::hbb_enum::csv_hbb, stored_csvs.store[0].second->p4() + stored_csvs.store[1].second->p4());
      if (stored_cmvas.store[1].second)
        output.set_hbb(hbbfile::hbb_enum::cmva_hbb, stored_cmvas.store[0].second->p4() + stored_cmvas.store[1].second->p4());

      auto recoilvec = event.pfMet.v() + lepvec.Vect().XYvector();
      output.recoil = recoilvec.Mod();
      output.recoilphi = recoilvec.Phi();

      //// FILTER ////

      if (not (output.csv_hbb or output.cmva_hbb))
        continue;

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
