#include "input.h"

#include "regfile.h"
#include "feedregression.h"

#include "crombie/CmsswParse.h"

#include "JECCorrector.h"

int parsed_main(int argc, char** argv) {

  regfile output {argv[argc - 1]};

  panda::JECCorrector corrector {"data/jec/Autumn18_V8_MC", "AK4PFchs"};

  for (int i_file = 1; i_file < argc - 1; i_file++) {

    std::cout << "Running over file " << argv[i_file]
              << " (" << i_file << "/" << (argc - 2) << ")" << std::endl;

    // Get the PandaTree
    TFile input {argv[i_file]};
    auto* events_tree = static_cast<TTree*>(input.Get("events"));
    panda::Event event;
    crombie::feedpanda(event, events_tree);
    auto nentries = input::maxevents ? input::maxevents : events_tree->GetEntries();

    for(decltype(nentries) entry = 0; entry != nentries; ++entry) {
      if (entry % 10000 == 0)
        std::cout << "Processing events: ... " << float(entry)/nentries*100 << " %" << std::endl;

      event.getEntry(*events_tree, entry);

      corrector.update_event(event, event.chsAK4Jets, event.pfMet);
      event.chsAK4Jets = corrector.get_jets();
      event.pfMet = corrector.get_met();

      output.reset(event);

      auto gennumap = gennujet::get_gen_nu_map(event);

      for (auto& jet : event.chsAK4Jets) {

        if (jet.pt() > 15 and std::abs(jet.eta() < 2.5)) {

          auto genjet = jet.matchedGenJet;

          if (genjet.isValid() and genjet->numB) {

            auto gennuvec = gennumap.at(genjet.get());

            output.set_jet(regfile::jet::Jet, jet, gennuvec, genjet);
            output.fill();

          }

        }

      }

    }

    input.Close();

  }

  return 0;

}


int main(int argc, char** argv) {
  return parse_then_send(argc, argv, parsed_main);
}
