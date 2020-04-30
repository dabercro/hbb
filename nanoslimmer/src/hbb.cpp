#include "feedhbb.h"
#include "hbbfile.h"
#include "cleaning.h"
#include "lepid.h"
#include "jetid.h"
#include "main.h"
#include "metphicorr.h"


void process_event(const std::string& year, hbbfile& output, const panda::Event& event) {

  output.reset(event, metphicorr::correctMETXY(event, event.MET, year),
               triggers::fired(event, year));

  auto lep_select = [&output] (const auto& leps) {

    const std::vector<hbbfile::lep> enums {hbbfile::lep::lep1, hbbfile::lep::lep2};

    // If leptons in the output tree already filled, skip filling other type
    unsigned i_enum = output.lep1 ? enums.size() : 0;

    for (auto& lep : leps) {
      if (lepid::loose(lep)) {
        output.n_lep_loose++;

        if (i_enum < enums.size())
          output.set_lep(enums[i_enum++], lep);

        if (lepid::tight(lep))
          output.n_lep_tight++;
      }
    }
    
  };

  lep_select(event.Muon);
  lep_select(event.Electron);

  const panda::FatJet* fatjet_ptr = nullptr;

  for (auto& fatjet : event.FatJet) {
    if (fatjet.pt > 200 && fatjet.msoftdrop > 50) {
      fatjet_ptr = &fatjet;
      output.set_fatjet(hbbfile::fatjet::fatjet1, fatjet);
      break;
    }
  }

  for (auto& jet : event.Jet) {
    if (cleaning::lepfilter(jet, event)) {
      output.num_jet += 1;

      if (jet.pt > 15 and jet.puId) {

        if (fatjet_ptr and jetid::medium_b(jet) and cleaning::outside(jet, *fatjet_ptr, 0.8))
          output.fatjet1_num_outside_b++;

        if (not output.jet2)
          output.set_jet(output.jet1
                         ? hbbfile::jet::jet2
                         : hbbfile::jet::jet1,
                         jet);

      }
    }
  }

  output.fill();

}


int main(int argc, char** argv) {
  return main_wrapper<hbbfile>(argc, argv);
}
