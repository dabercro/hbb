#include "smearfile.h"
#include "checkrun.h"
#include "feedsmear.h"

#include "crombie/CmsswParse.h"

#include "TH1F.h"

void process_event(smearfile& output, const panda::Event& event) {

  if (not checkrun(event.run,
                   event.luminosityBlock))
    return;

  // Do the stuff

  if (event.Muon.size() < 2)
    return;

  output.reset(event);

  output.set_lep(smearfile::lep::lep1, event.Muon.at(0));
  output.set_lep(smearfile::lep::lep2, event.Muon.at(1));

  unsigned num_jet = 0;

  for (auto& jet : event.Jet) {
    if (jet.pt > 15 and jet.puId) {

      if (not output.jet2)
        output.set_jet(output.jet1
                       ? smearfile::jet::jet2
                       : smearfile::jet::jet1,
                       jet);

      num_jet++;

    }
  }

  // if (num_jet > 2)
  //   return;

  output.fill();

}

template <typename T> 
int parsed_main(int argc, char** argv) {

  T output {argv[argc - 1]};
  TH1F all_hist {"htotal", "htotal", 1, -1, 1};

  // Loop over all input files
  for (int i_file = 1; i_file < argc - 1; i_file++) {

    std::cout << "Running over file " << argv[i_file]
              << " (" << i_file << "/" << (argc - 2) << ")" << std::endl;

    // Get the PandaTree
    TFile input {argv[i_file]};
    auto* events_tree = static_cast<TTree*>(input.Get("Events"));
    panda::Event event;
    crombie::feedpanda(event, events_tree);
    auto nentries = events_tree->GetEntries();

    // Loop over tree
    for(decltype(nentries) entry = 0; entry != nentries; ++entry) {

      if (entry % 100000 == 0)
        std::cout << "Processing events: ... " << float(entry)/nentries*100 << " %" << std::endl;

      event.getEntry(*events_tree, entry);
      all_hist.Fill(0.0, event.genWeight);

      process_event(output, event);

    }

    input.Close();
  }

  output.write(&all_hist);
  return 0;

}


int main(int argc, char** argv) {
  return parse_then_send(argc, argv, parsed_main<smearfile>);
}
