#ifndef HBBNANO_MAIN_H
#define HBBNANO_MAIN_H


#include "PandaTree/Objects/interface/Event.h"

#include "checkrun.h"
#include "feednano.h"
#include "TH1F.h"
#include "TFile.h"


template <typename T> 
int main_wrapper(int argc, char** argv) {

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
    crombie::feednano(event, events_tree);
    auto nentries = events_tree->GetEntries();

    // Loop over tree
    for(decltype(nentries) entry = 0; entry != nentries; ++entry) {

      if (entry % 100000 == 0)
        std::cout << "Processing events: ... " << float(entry)/nentries*100 << " %" << std::endl;

      event.getEntry(*events_tree, entry);
      all_hist.Fill(0.0, event.genWeight);

      if (checkrun(event.run,
                   event.luminosityBlock))
        process_event(output, event);

    }

    input->Close();
    delete input;
  }

  output.write(&all_hist);
  return output_code;

}


#endif
