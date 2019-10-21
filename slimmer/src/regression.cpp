#include "regfile.h"
#include "feedregression.h"
#include "main.h"

#include "crombie/CmsswParse.h"

template <>
void process_event<regfile>(regfile& output, const panda::Event& event, jetselect::JetSelector& jetselector) {

  auto updated_jets = jetselector.update_event();

  output.reset(event, updated_jets.met);

  auto gennumap = gennujet::get_gen_nu_map(event);

  for (auto& jet : updated_jets.ak4jets) {

    if (jet.pt() > 15 and std::abs(jet.eta() < 2.5)) {

      auto& genjet = jet.matchedGenJet;

      if (genjet.isValid() and genjet->numB) {

        auto gennuvec = gennumap.at(genjet.get());

        output.set_jet(regfile::jet::Jet, jet, gennuvec, genjet);
        output.fill();

      }
    }
  }
}


int main(int argc, char** argv) {
  return parse_then_send(argc, argv, parsed_main<regfile>);
}
