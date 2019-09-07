#include "exceptions.h"
#include "jetselect.h"
#include "smearfile.h"
#include "checkrun.h"
#include "triggers.h"
#include "feedsmear.h"
#include "puid.h"

#include "TH1F.h"

#include "crombie/CmsswParse.h"


int parsed_main(int argc, char** argv) {

  smearfile output {argv[argc - 1]};
  TH1F all_hist {"htotal", "htotal", 1, -1, 1};

  jetselect::JetSelector jetselector {};

  // Loop over all input files
  for (int i_file = 1; i_file < argc - 1; i_file++) {

    std::cout << "Running over file " << argv[i_file]
              << " (" << i_file << "/" << (argc - 2) << ")" << std::endl;

    // Get the PandaTree
    TFile input {argv[i_file]};
    auto* events_tree = static_cast<TTree*>(input.Get("events"));
    panda::Event event;
    crombie::feedpanda(event, events_tree);
    auto nentries = input::maxevents ? input::maxevents : events_tree->GetEntries();

    triggers::init(event);
    jetselector.init(event);

    for(decltype(nentries) entry = 0; entry != nentries; ++entry) {

      event.getEntry(*events_tree, entry);
      all_hist.Fill(0.0, event.weight);

      if (not checkrun(event.runNumber,
                       event.lumiNumber))
        continue;

      // Do the stuff

      leptonselect::SelectedLeptons selectedleptons {event};

      if (selectedleptons.loose.size() != 2)
        continue;

      auto updated_jets = jetselector.update_event();

      if (not updated_jets.ak4jets.size())
        continue;

      output.reset(event, updated_jets.pfmet);

      output.set_lep(smearfile::lep::lep1, exceptions::at(selectedleptons.loose, 0, __LINE__));
      output.set_lep(smearfile::lep::lep2, exceptions::at(selectedleptons.loose, 1, __LINE__));

      unsigned num_jet = 0;

      for (auto& jet : updated_jets.ak4jets)
        if (jet.pt() > 15 and jetselect::clean_jet(jet, selectedleptons) and puid::loose(jet)) {

          if (not output.jet2)
            output.set_jet(output.jet1
                           ? smearfile::jet::jet2
                           : smearfile::jet::jet1,
                           jet);

          num_jet++;

        }

      if (num_jet > 2)
        continue;

      output.fill();
    }
  }

  output.write(&all_hist);
  return 0;

}


int main(int argc, char** argv) {
  return parse_then_send(argc, argv, parsed_main);
}
