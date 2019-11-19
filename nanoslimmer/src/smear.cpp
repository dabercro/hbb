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
    if (lepid::Z(muon)) {

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

    // jetsForHiggs = [x for x in jets if x.lepFilter and x.puId>0 and x.jetId>0 and self.pt(x,self.isMC)>20 and abs(x.eta)<2.5]

    // The eta cut will come later at the analysis level for the smearing.
    // We really want only two jets.

    if (cleaning::lepfilter(jet, event) and
        jet.puId > 0 and jet.jetId > 0 and
        jet.pt * jet.bRegCorr > 20) {

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
