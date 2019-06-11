#include <iostream>

#include "PandaTree/Objects/interface/Event.h"

#include "example.h"

#include "TLorentzVector.h"

int main (int argc, char* argv[]) {

  if (argc != 3) {
    std::cerr << "USAGE: " << argv[0] << " INPUT OUTPUT" << std::endl;
    return 1;
  }

  TFile input {argv[1]};
  auto* events_tree = static_cast<TTree*>(input.Get("events"));
  panda::Event event;
  event.setAddress(*events_tree);

  example output {argv[2]};

  Long64_t nentries = events_tree->GetEntries();

  for(Long64_t i_entry = 0; i_entry < nentries; ++i_entry) {
    if (i_entry % 10000 == 0)
      std::cout << "Processing events: ... " << float(i_entry)/nentries*100 << " %" << std::endl;

    event.getEntry(*events_tree, i_entry);
    output.reset();

    TLorentzVector m1;
    TLorentzVector m2;

    output.n_lep = event.muons.size();

    for (auto& muon : event.muons) {

      if (not output.muon1) {
        output.set_muon(example::muon::muon1, muon);
        m1 = muon.p4();
      }

      else if (not output.muon2) {
        output.set_muon(example::muon::muon2, muon);
        m2 = muon.p4();
      }

    }

    if (output.muon2) {

      TLorentzVector dimuon = m1 + m2;

      output.dimuon_m = dimuon.M();
      output.dimuon_pt = dimuon.Pt();

      output.fill();

    }

  }

  return 0;

}
