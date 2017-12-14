#include <algorithm>
#include <iostream>
#include <utility>

#include "hbbfile.h"

#include "SkimmingTools/interface/CmsswParse.h"
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
    event.setAddress(*events_tree);
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

      int lep_pdgid = 0;
      using leppair = std::pair<panda::Lepton*, panda::Lepton*>;
      leppair top_muons;
      leppair top_eles;

      // Define default checks here
      auto check_lep = [&output, &lepvec] (leppair& top, panda::Lepton& lep) {
        if (lep.loose) {
          lepvec += lep.p4();
          output.n_looselep++;
          check_top_two(lep, top, [](panda::Lepton* l) {return l->pt();});
          if (lep.medium) {
            output.n_mediumlep++;
            if (lep.tight)
              output.n_tightlep++;
          }
        }
      };

      // Loop over muons
      for (auto& lep : event.muons)
        check_lep(top_muons, lep);

      // Loop over electrons
      for (auto& lep : event.electrons)
        check_lep(top_eles, lep);

      auto set_lep = [&output] (panda::Lepton* top_lep, hbbfile::lep_enum lep) {
        if (top_lep)
          output.set_lep(lep, *top_lep);
      };

      set_lep(top_muons.first, hbbfile::lep_enum::muon1);
      set_lep(top_muons.first, hbbfile::lep_enum::muon2);
      set_lep(top_eles.first, hbbfile::lep_enum::ele1);
      set_lep(top_eles.first, hbbfile::lep_enum::ele2);

      //// JETS ////

      // We want the two jets with the highest CSV and CMVA
      using jetpair = std::pair<panda::Jet*, panda::Jet*>;
      jetpair top_jets;
      jetpair top_csvs;
      jetpair top_cmvas;

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

        check_top_two(jet, top_jets, [](panda::Jet* j) {return j->pt();});
        check_top_two(jet, top_csvs, [](panda::Jet* j) {return j->csv;});
        check_top_two(jet, top_cmvas, [](panda::Jet* j) {return j->cmva;});
      }

      auto set_jet = [&output] (panda::Jet* top_jet, hbbfile::jet_enum jet) {
        if (top_jet) {
          output.set_jet(jet, *top_jet);
          auto& gen = top_jet->matchedGenJet;
          if (gen.isValid())
            output.set_genjet(jet, *gen);
        }
      };

      set_jet(top_jets.first, hbbfile::jet_enum::jet1);
      set_jet(top_jets.second, hbbfile::jet_enum::jet2);

      // Includes getting secondary vertex and leading leptons
      auto set_bjet = [&output, &set_jet] (panda::Jet* top_jet, hbbfile::jet_enum jet, hbbfile::bjet_enum bjet) {
        set_jet(top_jet, jet);
        if (top_jet) {
          auto& vert = top_jet->secondaryVertex;
          if (vert.isValid())
            output.set_bjet(bjet, *vert);

          int nlep = 0;
          const panda::PFCand* maxlep = nullptr;

          decltype(maxlep->pt()) maxtrkpt = 0;

          for (auto pf : top_jet->constituents) {
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
            output.set_bleps(bjet, *top_jet, nlep, *maxlep);
        }
      };

      set_bjet(top_csvs.first, hbbfile::jet_enum::csv_jet1, hbbfile::bjet_enum::csv_jet1);
      set_bjet(top_csvs.second, hbbfile::jet_enum::csv_jet2, hbbfile::bjet_enum::csv_jet2);
      set_bjet(top_cmvas.first, hbbfile::jet_enum::cmva_jet1, hbbfile::bjet_enum::cmva_jet1);
      set_bjet(top_cmvas.second, hbbfile::jet_enum::cmva_jet2, hbbfile::bjet_enum::cmva_jet2);

      if (top_csvs.second)
        output.set_hbb(hbbfile::hbb_enum::csv_hbb, top_csvs.first->p4() + top_csvs.second->p4());
      if (top_cmvas.second)
        output.set_hbb(hbbfile::hbb_enum::cmva_hbb, top_cmvas.first->p4() + top_cmvas.second->p4());

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
