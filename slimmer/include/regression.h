// Get all the variables for regression

namespace regression {

  // Get the directions of the PUPPI particles in the area
  class JetInfo {
  public:
    using type = TLorentzVector;
    using scalar = float;

    // PUPPI vectors
    type charged {};
    type neutral {};
    type charged_pu {};
    type neutral_pu {};

    // Rings each are separate vector element
    using rings = std::vector<std::pair<scalar, type>>;
    rings em_rings = rings(5);
    rings neut_rings = rings(5);
    rings ch_rings = rings(5);
    rings mu_rings = rings(5);

    // Additional information about the pf candidates
    scalar ptD {0};
    unsigned num03 {0};
  };

  JetInfo GetJetInfo(const panda::Jet& jet) {
    JetInfo output {};

    // For the ptD calculation
    float sum_weight {0};
    float sum_pt {0};

    // Fill information about the constituents
    for (auto pf : jet.constituents) {
      auto p4 = pf->p4();

      // PUPPI
      auto puppi = pf->puppiW();
      if (pf->q()) {
        output.charged += p4 * puppi;
        output.charged_pu += p4 * (1.0 - puppi);
      }
      else {
        output.neutral += p4 * puppi;
        output.neutral_pu += p4 * (1.0 - puppi);
      }

      // Energy Rings
      static const std::vector<double> dr_bins {
        std::pow(0.05, 2), std::pow(0.1, 2), std::pow(0.2, 2), std::pow(0.3, 2), std::pow(0.4, 2)
      };
      unsigned bin = std::lower_bound(dr_bins.begin(), dr_bins.end(),
                                      KinematicFunctions::deltaR2(p4.Eta(), p4.Phi(), jet.eta(), jet.phi())) - dr_bins.begin();
      if (bin < dr_bins.size()) {
        // Get which ring offset
        auto rings = [&pf] () {
          unsigned pdgid = std::abs(pf->pdgId());
          if (pdgid == 22 || pdgid == 11)
            return &JetInfo::em_rings;
          else if (pdgid == 13)
            return &JetInfo::mu_rings;
          else if (pf->q())
            return &JetInfo::ch_rings;
          return &JetInfo::neut_rings;
        } ();
        // Apply info
        (output.*rings)[bin].first += p4.E();
        (output.*rings)[bin].second += p4;
      }

      // ptD
      auto pt = p4.Pt();
      sum_weight += std::pow(pt, 2);
      sum_pt += pt;
      // num candidates with pt > 0.3
      if (pt > 0.3)
        ++output.num03;
    }

    output.ptD = sum_weight/sum_pt;

    return output;
  }
}
