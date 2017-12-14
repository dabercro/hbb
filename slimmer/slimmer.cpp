#include <iostream>
#include <utility>

#include "hbbfile.h"

#include "SkimmingTools/interface/CmsswParse.h"
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
    event.setAddress(*events_tree);
    auto nentries = events_tree->GetEntries();

    // Loop over tree
    for(decltype(nentries) entry = 0; entry != nentries; ++entry) {
      if (entry % 10000 == 0)
        std::cout << "Processing events: ... " << float(entry)/nentries*100 << " %" << std::endl;
      event.getEntry(*events_tree, entry);
      output.reset(event);

      // We want the two jets with the highest CSV and CMVA
      using jetpair = std::pair<panda::Jet*, panda::Jet*>;
      jetpair top_csvs;
      jetpair top_cmvas;

      // Loop over AK4CHS jets
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

        auto check_b_jet = [&jet] (jetpair& top, float(*get_tag)(panda::Jet*)) {
          auto tag = get_tag(&jet);
          if (!top.first or tag > get_tag(top.first)) {
            top.second = top.first;
            top.first = &jet;
          }
          else if (!top.second or tag > top.second->csv)
            top.second = &jet;
        };

        check_b_jet(top_csvs, [](panda::Jet* j) {return j->csv;});
        check_b_jet(top_cmvas, [](panda::Jet* j) {return j->cmva;});

        if (output.n_jet == 1)
          output.set_jet(hbbfile::set_jet_enum::jet1, jet);
        else if (output.n_jet == 2)
          output.set_jet(hbbfile::set_jet_enum::jet2, jet);
      }

      auto set_b_jet = [&output] (panda::Jet* top_jet, hbbfile::set_jet_enum set_jet, hbbfile::set_b_jet_enum set_b_jet) {
        if (top_jet) {
          output.set_jet(set_jet, *top_jet);
          auto& vert = top_jet->secondaryVertex;
          if (vert.isValid())
            output.set_b_jet(set_b_jet, vert);

          float maxpt = 0;
          int nlep = 0;
          panda::Ref<panda::PFCand>* maxpf = nullptr;
          for (auto pf : top_jet->constituents) {
            if (pf->q()) {
              auto pdgid = abs(pf->pdgId());
              if (pdgid == 11 || pdgid == 13) {
              }
            }
          }
        }
      };

      set_b_jet(top_csvs.first, hbbfile::set_jet_enum::csv_jet1, hbbfile::set_b_jet_enum::csv_jet1);
      set_b_jet(top_csvs.second, hbbfile::set_jet_enum::csv_jet2, hbbfile::set_b_jet_enum::csv_jet2);
      set_b_jet(top_cmvas.first, hbbfile::set_jet_enum::cmva_jet1, hbbfile::set_b_jet_enum::cmva_jet1);
      set_b_jet(top_cmvas.second, hbbfile::set_jet_enum::cmva_jet2, hbbfile::set_b_jet_enum::cmva_jet2);

      if (top_csvs.second)
        output.set_hbb(hbbfile::set_hbb_enum::csv_hbb, top_csvs.first->p4() + top_csvs.second->p4());
      if (top_cmvas.second)
        output.set_hbb(hbbfile::set_hbb_enum::cmva_hbb, top_cmvas.first->p4() + top_cmvas.second->p4());

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
