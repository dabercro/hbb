#include "smearfile.h"
#include "cleaning.h"
#include "lepid.h"
#include "main.h"
#include "JECCorrector.h"


void process_event(smearfile& output, panda::Event& event) {

  // Do the stuff

  if (event.Muon.size() < 2)
    return;

  panda::JECCorrector::adjust_event(event);

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
    if (jet.pt > 15 and jet.puId and cleaning::cleanjet(jet, event)) {

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


int main(int argc, char** argv) {
  return main_wrapper<smearfile>(argc, argv);
}
