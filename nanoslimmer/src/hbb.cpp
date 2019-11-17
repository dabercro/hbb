#include "hbbfile.h"
#include "cleaning.h"
#include "lepid.h"
#include "main.h"


void process_event(hbbfile& output, const panda::Event& event) {

  output.reset(event);

  auto lep_select = [&output] (const auto& leps) {

    const std::vector<hbbfile::lep> enums {hbbfile::lep::lep1, hbbfile::lep::lep2};

    // If leptons in the output tree already filled, skip filling other type
    unsigned i_enum = output.lep1 ? enums.size() : 0;

    for (auto& lep : leps) {
      if (lepid::presel(lep)) {
        output.n_lep_presel++;
        if (lepid::loose(lep)) {
          output.n_lep_loose++;

          if (i_enum < enums.size())
            output.set_lep(enums[i_enum++], lep);

          if (lepid::tight(lep))
            output.n_lep_tight++;
        }
      }
    }
    
  };

  lep_select(event.Muon);
  lep_select(event.Electron);

  for (auto& jet : event.Jet) {
    if (jet.pt > 15 and jet.puId and cleaning::cleanjet(jet, event)) {

      if (not output.jet2)
        output.set_jet(output.jet1
                       ? hbbfile::jet::jet2
                       : hbbfile::jet::jet1,
                       jet);

      output.num_jet++;

    }
  }

  output.fill();

}


int main(int argc, char** argv) {
  return main_wrapper<hbbfile>(argc, argv);
}
