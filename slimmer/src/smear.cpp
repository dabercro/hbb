#include "jetselect.h"
#include "smearfile.h"
#include "checkrun.h"
#include "triggers.h"
#include "feedsmear.h"

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

      output.reset(event);

      // Do the stuff

      output.fill();
    }
  }

  output.write(&all_hist);
  return 0;

}


int main(int argc, char** argv) {
  return parse_then_send(argc, argv, parsed_main);
}
