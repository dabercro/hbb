#include "feedsmear.h"
#include "smearfile.h"
#include "cleaning.h"
#include "lepid.h"
#include "jetid.h"
#include "main.h"
#include "JECCorrector.h"


void process_event(int year, smearfile& output, panda::Event& event) {

  // Do the stuff

  if (event.Muon.size() < 2 and event.Electron.size() < 2)
    return;

  panda::JECCorrector::adjust_event(event, year);

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

  if (not output.lep1) {

    for (auto& electron : event.Electron) {
      if (lepid::Z(electron)) {

        if (not output.lep2)
          output.set_lep(output.lep1
                         ? smearfile::lep::lep2
                         : smearfile::lep::lep1,
                         electron);

        else
          return;

      }
    }

  }

  for (auto& jet : event.Jet) {

    // The eta cut will come later at the analysis level for the smearing.
    // We really want only two jets.

    if (cleaning::lepfilter(jet, event)) {

      output.num_jet++;

      if (jetid::higgs(jet, event)) {

        output.num_bjet++;

        if (not output.jet2)
          output.set_jet(output.jet1
                         ? smearfile::jet::jet2
                         : smearfile::jet::jet1,
                         jet);

      }
    }
  }

  output.fill();

}


int main(int argc, char** argv) {
  return main_wrapper<smearfile>(argc, argv);
}
