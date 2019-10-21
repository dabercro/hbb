#include "main.h"
#include "exceptions.h"
#include "smearfile.h"
#include "checkrun.h"
#include "triggers.h"
#include "feedsmear.h"
#include "puid.h"

#include "crombie/CmsswParse.h"


template <>
void process_event<smearfile>(smearfile& output, const panda::Event& event, jetselect::JetSelector& jetselector) {

  if (not checkrun(event.runNumber,
                   event.lumiNumber))
    return;

  // Do the stuff

  leptonselect::SelectedLeptons selectedleptons {event};

  if (selectedleptons.loose.size() != 2)
    return;

  auto updated_jets = jetselector.update_event();

  if (not updated_jets.ak4jets.size())
    return;

  output.reset(event, updated_jets.met);

  output.set_lep(smearfile::lep::lep1, exceptions::at(selectedleptons.loose, 0, __LINE__));
  output.set_lep(smearfile::lep::lep2, exceptions::at(selectedleptons.loose, 1, __LINE__));

  unsigned num_jet = 0;

  for (auto& jet : updated_jets.ak4jets)
    if (jet.pt() > 15 and jetselect::clean_jet(jet, selectedleptons) and puid::loose(jet)) {

      output.set_bsf(jet);

      if (not output.jet2)
        output.set_jet(output.jet1
                       ? smearfile::jet::jet2
                       : smearfile::jet::jet1,
                       jet);

      num_jet++;

    }

  if (num_jet > 2)
    return;

  for (auto& gen : event.genParticles) {

    if (input::version >= 12 and gen.miniaodPacked)
      continue;
    if (not output.gen and gen.pdgid == 23)
      output.set_gen(gen);

  }

  output.fill();
}


int main(int argc, char** argv) {
  return parse_then_send(argc, argv, parsed_main<smearfile>);
}
