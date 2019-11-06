#include "smearfile.h"
#include "checkrun.h"
#include "feedsmear.h"
#include "lepid.h"

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

  for (auto& muon : event.Muon) {
    if (lepid::loose(muon)) {

      if (not output.lep2)
        output.set_lep(output.lep1
                       ? smearfile::lep::lep2
                       : smearfile::lep::lep1,
                       muon);

      else
        return;

    }
  }

  for (auto& jet : event.Jet) {
    if (jet.pt > 15 and jet.puId and jet.cleanmask) {

      if (not output.jet2)
        output.set_jet(output.jet1
                       ? smearfile::jet::jet2
                       : smearfile::jet::jet1,
                       jet);

      output.num_jet++;

    }
  }

  output.fill();

}

template <typename T> 
int parsed_main(int argc, char** argv) {

  int output_code = 0;

  T output {argv[argc - 1]};
  TH1F all_hist {"htotal", "htotal", 1, -1, 1};

  // Loop over all input files
  for (int i_file = 1; i_file < argc - 1; i_file++) {

    std::cout << "Running over file " << argv[i_file]
              << " (" << i_file << "/" << (argc - 2) << ")" << std::endl;

    // Get the PandaTree
    auto* input = TFile::Open(argv[i_file]);

    if (not input) {
      std::cerr << "Could not open " << argv[i_file] << std::endl;
      output_code += 1;
      continue;
    }

    auto* events_tree = static_cast<TTree*>(input->Get("Events"));
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

    input->Close();
    delete input;
  }

  output.write(&all_hist);
  return output_code;

}


int main(int argc, char** argv) {
  return parsed_main<smearfile>(argc, argv);
}
