#include "input.h"

#include "regfile.h"
#include "feedregression.h"

#include "crombie/CmsswParse.h"

int parsed_main(int argc, char** argv) {

  regfile output {argv[argc - 1]};

  for (int i_file = 1; i_file < argc - 1; i_file++) {

    std::cout << "Running over file " << argv[i_file]
              << " (" << i_file << "/" << (argc - 2) << ")" << std::endl;

    // Get the PandaTree
    TFile input {argv[i_file]};
    auto* events_tree = static_cast<TTree*>(input.Get("events"));
    panda::Event event;
    feedpanda(event, events_tree);
    auto nentries = input::maxevents ? input::maxevents : events_tree->GetEntries();

    for(decltype(nentries) entry = 0; entry != nentries; ++entry) {
      if (entry % 10000 == 0)
        std::cout << "Processing events: ... " << float(entry)/nentries*100 << " %" << std::endl;

      event.getEntry(*events_tree, entry);
      output.reset(event);

      for (auto& jet : event.chsAK4Jets) {

        output.set_jet(regfile::jet::Jet, jet);

        output.fill();

        break;

      }

    }

    input.Close();

  }

  return 0;

}


int main(int argc, char** argv) {
  return parse_then_send(argc, argv, parsed_main);
}
