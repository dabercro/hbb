// Here we deal with particle flow stuff

namespace {
  static EtaPhiMap<fastjet::PseudoJet> nujetmap {0.5, 3.5};
  static std::vector<fastjet::PseudoJet> nujets;
}

namespace pfcands {
  static EtaPhiMap<panda::PFCand> pfmap {0.1};

  void MakeNuJets (panda::Event& event) {
    if (event.isData)
      return;

    std::vector<fastjet::PseudoJet> inputs;
    inputs.reserve(event.pfCandidates.size());

    for (auto& gen : event.genParticles) {
      if (gen.finalState and gen.pt() > 0.001) {
        auto abspdgid = std::abs(gen.pdgid);
        if ((abspdgid == 12 or abspdgid == 14) and std::abs(gen.eta()) < 3.5) {
          auto& parent = gen.parent;
          // Don't add neutrinos from massive W to jet
          if (parent.isValid() and abs(parent->pdgid) == 24 and parent->m() > 50)
            continue;
        }
        inputs.emplace_back(gen.px(), gen.py(), gen.pz(), gen.e());
      }
    }

    auto sequence = fastjet::ClusterSequence(inputs, {fastjet::JetAlgorithm::antikt_algorithm, 0.4});
    nujets = sequence.inclusive_jets(15.0); // Only want pT > 15.0
    if (debug::debug)
      std::cout << "Have " << nujets.size() << " jets" << std::endl;
    nujetmap.AddParticles(nujets);
  }

  const fastjet::PseudoJet* NuJet (const panda::Jet& jet) {
    float mindr2 = 10.0;
    const fastjet::PseudoJet* output;
    for (auto* nujet : nujetmap.GetParticles(jet.eta(), jet.phi(), 0.4)) {
      auto check = deltaR2(jet.eta(), jet.phi(), nujet->eta(), nujet->phi());
      if (check < mindr2) {
        mindr2 = check;
        output = nujet;
      }
    }
    if (debug::debug)
      std::cout << "Getting nujet at " << output << std::endl;
    return output;
  }

}
