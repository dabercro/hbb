#ifndef HBBNANO_MAIN_H
#define HBBNANO_MAIN_H


#include "PandaTree/Objects/interface/Event.h"

#include "checkrun.h"
#include "TH1F.h"
#include "TFile.h"
#include "applysmearing.h"

#include <ctime>
#include <string>


template <typename T> 
int main_wrapper(int argc, char** argv) {

  auto start_time = std::time(nullptr);

  int output_code = 0;

  std::string year {argv[1]};

  std::cout << "Setting year to: " << year << std::endl;

  applysmearing::load(year);

  T output {argv[argc - 1]};
  TH1F all_hist {"htotal", "htotal", 1, -1, 1};

  // Loop over all input files
  for (int i_file = 2; i_file < argc - 1; i_file++) {

    std::cout << "Running over file " << argv[i_file]
              << " (" << i_file - 1 << "/" << (argc - 3) << ")" << std::endl;

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
        process_event(year, output, event);

    }

    input->Close();
    delete input;
  }

  output.write(&all_hist);

  auto end_time = std::time(nullptr);

  std::cout << "Ran over " << (argc - 3) << " file"
            << (argc == 4 ? "" : "s") << " in "
            << (end_time - start_time) << " seconds." << std::endl;

  return output_code;

}


#endif
