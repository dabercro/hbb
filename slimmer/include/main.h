#ifndef HBB_MAIN_H
#define HBB_MAIN_H 1


#include <iostream>

#include "triggers.h"
#include "jetselect.h"
#include "input.h"

#include "TH1F.h"
#include "TH1D.h"


template <typename T>
void process_event(T& output, const panda::Event& event, jetselect::JetSelector& jetselector);


namespace crombie {

  void feedpanda(panda::Event& event, TTree* input);

}


template <typename T> 
int parsed_main(int argc, char** argv) {

  T output {argv[argc - 1]};
  TH1F all_hist {"htotal", "htotal", 1, -1, 1};
  TH1D sums;

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

    // Get the hSumW
    auto* sumW = static_cast<TH1D*>(input.Get("hSumW"));
    if (sums.GetNbinsX() == 1)  // SumW has 8 bins, but default constructor makes 1
      sums = *sumW;
    else
      sums.Add(sumW);

    // Loop over tree
    for(decltype(nentries) entry = 0; entry != nentries; ++entry) {

      if (entry % 10000 == 0)
        std::cout << "Processing events: ... " << float(entry)/nentries*100 << " %" << std::endl;

      event.getEntry(*events_tree, entry);
      all_hist.Fill(0.0, event.weight);

      if (not entry)
        myrandom::gen.SetSeed(event.eventNumber);

      process_event(output, event, jetselector);

    }

    input.Close();
  }

  output.write(&all_hist);
  output.write(&sums);
  return 0;

}


#endif
