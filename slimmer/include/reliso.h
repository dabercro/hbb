
namespace {

  class MiniIsoParams {
  public:
    using type = double;
    type min_dr;
    type max_dr;
    type kt_scale;
    type pt_threshold;
    type deadcone_ch;
    type deadcone_pu;
    type deadcone_ph;
    type deadcone_nh;
    // Note that dz cut was taken out since it seems to always be zero
  };

  const MiniIsoParams muon_params {0.05, 0.2, 10.0, 0.5, 0.0001, 0.01, 0.01, 0.01};
  const MiniIsoParams ele_endcap {0.05, 0.2, 10.0, 0.0, 0.015, 0.015, 0.08, 0.0};
  const MiniIsoParams ele_barrel {0.05, 0.2, 10.0, 0.0, 0.000, 0.000, 0.00, 0.0};

}

namespace reliso {
  // Requires the relisomap to be filled with the event's PFCandidates
  // Only use rho for muon calculations
  double minireliso(panda::Lepton& lep, decltype(panda::Event::rho) rho = 0) {
    // Get the parameter set
    const auto& params = rho ? muon_params : (std::abs(lep.eta()) < 1.4442 ? ele_barrel : ele_endcap);

    double chiso = 0;
    double nhiso = 0;
    double phiso = 0;
    double puiso = 0;

    // Copy of pat::miniIsoDr
    double drcut = std::max(params.min_dr, std::min(params.max_dr, params.kt_scale/lep.pt()));

    for (const auto* cand : pfcands::pfmap.GetParticles(lep.eta(), lep.phi(), drcut)) {
      auto id = std::abs(cand->pdgId());
      auto pt = cand->pt();
      auto dr = deltaR(lep.eta(), lep.phi(), cand->eta(), cand->phi());
      switch(id) {
      case 211:
        if (cand->vertex.idx() == 0) {
          if (dr > params.deadcone_ch)
            chiso += pt;
        }
        else {
          if (pt > params.pt_threshold && dr > params.deadcone_pu)
            puiso += pt;
        }
        break;
      case 130:
        if (pt > params.pt_threshold && dr > params.deadcone_nh)
          nhiso += pt;
        break;
      case 22:
        if (pt > params.pt_threshold && dr > params.deadcone_ph)
          phiso += pt;
        break;
      // No action on default
      }
    }

    if (debug::debug) {
      std::cout << "Lepton pt " << lep.pt()
                << " chiso " << chiso
                << " nhiso " << nhiso
                << " phiso " << phiso
                << " puiso " << puiso << std::endl;
    }

    if (rho) {
      auto abseta = std::abs(lep.eta());
      auto ea = 0.0;
      if      (abseta < 0.800) ea = 0.0735;
      else if (abseta < 1.300) ea = 0.0619;
      else if (abseta < 2.000) ea = 0.0465;
      else if (abseta < 2.200) ea = 0.0433;
      else if (abseta < 2.500) ea = 0.0577;
      auto correction = rho * ea * std::pow(drcut/0.3, 2);
      return (chiso + std::max(0.0, nhiso + phiso - correction))/lep.pt();
    }

    return (chiso + std::max(0.0, nhiso + phiso - puiso))/lep.pt();
  }
}
