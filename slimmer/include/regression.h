// Get all the variables for regression

namespace regression {

  // Get the directions of the PUPPI particles in the area
  class PuppiInfo {
  public:
    using type = TLorentzVector;
    using scalar = float;
    // Charged part of jet
    type charged {};
    // Neutral part of jet
    type neutral {};
    // Charged pileup
    type charged_pu {};
    // Neutral pileup
    type neutral_pu {};

    // Rings each are separate vector element
    using rings = std::vector<std::pair<scalar, type>>;
    rings em_rings = rings(5);
    rings neut_rings = rings(5);
    rings ch_rings = rings(5);
    rings mu_rings = rings(5);
  };

  PuppiInfo GetPuppiInfo(const panda::Jet& jet) {
    PuppiInfo output {};

    for (auto pf : jet.constituents) {
      auto p4 = pf->p4();
      auto puppi = pf->puppiW();
      if (pf->q()) {
        output.charged += p4 * puppi;
        output.charged_pu += p4 * (1.0 - puppi);
      }
      else {
        output.neutral += p4 * puppi;
        output.neutral_pu += p4 * (1.0 - puppi);
      }

      // Get the index of the ring to add the energy to
      static const std::vector<double> dr_bins {
        std::pow(0.05, 2), std::pow(0.1, 2), std::pow(0.2, 2), std::pow(0.3, 2), std::pow(0.4, 2)
      };
      unsigned bin = std::lower_bound(dr_bins.begin(), dr_bins.end(),
                                      deltaR2(p4.Eta(), p4.Phi(), jet.eta(), jet.phi())) - dr_bins.begin();

      if (bin < dr_bins.size()) {
        // Get which ring offset
        auto rings = [&pf] () {
          unsigned pdgid = std::abs(pf->pdgId());
          if (pdgid == 22 || pdgid == 11)
            return &PuppiInfo::em_rings;
          else if (pdgid == 13)
            return &PuppiInfo::mu_rings;
          else if (pf->q())
            return &PuppiInfo::ch_rings;
          return &PuppiInfo::neut_rings;
        } ();
        // Apply info
        (output.*rings)[bin].first += p4.E();
        (output.*rings)[bin].second += p4;
      }
    }

    return output;
  }
}
