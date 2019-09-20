#ifndef DAN_QUICKB_H
#define DAN_QUICKB_H 1


#include <map>
#include <string>

#include "input.h"

#include "TH1F.h"
#include "TFile.h"

#include "SkimmingTools/interface/Correction.h"

#include "PandaTree/Objects/interface/Event.h"

namespace quickb {

  unsigned numB (const panda::Jet& jet) {

    auto& gen = jet.matchedGenJet;
    return gen.isValid() ? gen->numB : 0;

  }

}


namespace {

  std::map<std::string, Correction<TH1F>> correctors {

    {"heavy", {"data/reweight_deepcsv_nosub.root", "reweight"}},
    {"light", {"data/reweight_deepcsv_nob_nosub.root", "reweight"}},

    {"heavy_pt_0_150", {"data/reweight.root", "heavy_pt_0_150_reweight"}},
    {"heavy_pt_150_250", {"data/reweight.root", "heavy_pt_150_250_reweight"}},
    {"heavy_pt_250_inf", {"data/reweight.root", "heavy_pt_250_inf_reweight"}},
    {"light_pt_0_150", {"data/reweight.root", "light_pt_0_150_reweight"}},
    {"light_pt_150_250", {"data/reweight.root", "light_pt_150_250_reweight"}},
    {"light_pt_250_inf", {"data/reweight.root", "light_pt_250_inf_reweight"}},

    {"heavy_pt_0_150_eta_0_1p4", {"data/reweight.root", "heavy_pt_0_150_eta_0_1p4_reweight"}},
    {"heavy_pt_150_250_eta_0_1p4", {"data/reweight.root", "heavy_pt_150_250_eta_0_1p4_reweight"}},
    {"heavy_pt_250_inf_eta_0_1p4", {"data/reweight.root", "heavy_pt_250_inf_eta_0_1p4_reweight"}},
    {"heavy_pt_0_150_eta_1p4_2p5", {"data/reweight.root", "heavy_pt_0_150_eta_1p4_2p5_reweight"}},
    {"heavy_pt_150_250_eta_1p4_2p5", {"data/reweight.root", "heavy_pt_150_250_eta_1p4_2p5_reweight"}},
    {"heavy_pt_250_inf_eta_1p4_2p5", {"data/reweight.root", "heavy_pt_250_inf_eta_1p4_2p5_reweight"}},
    {"light_pt_0_150_eta_0_1p4", {"data/reweight.root", "light_pt_0_150_eta_0_1p4_reweight"}},
    {"light_pt_150_250_eta_0_1p4", {"data/reweight.root", "light_pt_150_250_eta_0_1p4_reweight"}},
    {"light_pt_250_inf_eta_0_1p4", {"data/reweight.root", "light_pt_250_inf_eta_0_1p4_reweight"}},
    {"light_pt_0_150_eta_1p4_2p5", {"data/reweight.root", "light_pt_0_150_eta_1p4_2p5_reweight"}},
    {"light_pt_150_250_eta_1p4_2p5", {"data/reweight.root", "light_pt_150_250_eta_1p4_2p5_reweight"}},
    {"light_pt_250_inf_eta_1p4_2p5", {"data/reweight.root", "light_pt_250_inf_eta_1p4_2p5_reweight"}},

    {"heavy_pt_0_150_eta_0_1p4_190214", {"data/reweight_190214.root", "heavy_pt_0_150_eta_0_1p4_reweight"}},
    {"heavy_pt_150_250_eta_0_1p4_190214", {"data/reweight_190214.root", "heavy_pt_150_250_eta_0_1p4_reweight"}},
    {"heavy_pt_250_inf_eta_0_1p4_190214", {"data/reweight_190214.root", "heavy_pt_250_inf_eta_0_1p4_reweight"}},
    {"heavy_pt_0_150_eta_1p4_2p5_190214", {"data/reweight_190214.root", "heavy_pt_0_150_eta_1p4_2p5_reweight"}},
    {"heavy_pt_150_250_eta_1p4_2p5_190214", {"data/reweight_190214.root", "heavy_pt_150_250_eta_1p4_2p5_reweight"}},
    {"heavy_pt_250_inf_eta_1p4_2p5_190214", {"data/reweight_190214.root", "heavy_pt_250_inf_eta_1p4_2p5_reweight"}},
    {"light_pt_0_150_eta_0_1p4_190214", {"data/reweight_190214.root", "light_pt_0_150_eta_0_1p4_reweight"}},
    {"light_pt_150_250_eta_0_1p4_190214", {"data/reweight_190214.root", "light_pt_150_250_eta_0_1p4_reweight"}},
    {"light_pt_250_inf_eta_0_1p4_190214", {"data/reweight_190214.root", "light_pt_250_inf_eta_0_1p4_reweight"}},
    {"light_pt_0_150_eta_1p4_2p5_190214", {"data/reweight_190214.root", "light_pt_0_150_eta_1p4_2p5_reweight"}},
    {"light_pt_150_250_eta_1p4_2p5_190214", {"data/reweight_190214.root", "light_pt_150_250_eta_1p4_2p5_reweight"}},
    {"light_pt_250_inf_eta_1p4_2p5_190214", {"data/reweight_190214.root", "light_pt_250_inf_eta_1p4_2p5_reweight"}},

    // Reweighting for PUPPI Jets
    {"heavy_pt_0_150_eta_0_1p4_190916", {"data/reweight_190916.root", "heavy_pt_0_150_eta_0_1p4_reweight"}},
    {"heavy_pt_150_250_eta_0_1p4_190916", {"data/reweight_190916.root", "heavy_pt_150_250_eta_0_1p4_reweight"}},
    {"heavy_pt_250_inf_eta_0_1p4_190916", {"data/reweight_190916.root", "heavy_pt_250_inf_eta_0_1p4_reweight"}},
    {"heavy_pt_0_150_eta_1p4_2p5_190916", {"data/reweight_190916.root", "heavy_pt_0_150_eta_1p4_2p5_reweight"}},
    {"heavy_pt_150_250_eta_1p4_2p5_190916", {"data/reweight_190916.root", "heavy_pt_150_250_eta_1p4_2p5_reweight"}},
    {"heavy_pt_250_inf_eta_1p4_2p5_190916", {"data/reweight_190916.root", "heavy_pt_250_inf_eta_1p4_2p5_reweight"}},
    {"light_pt_0_150_eta_0_1p4_190916", {"data/reweight_190916.root", "light_pt_0_150_eta_0_1p4_reweight"}},
    {"light_pt_150_250_eta_0_1p4_190916", {"data/reweight_190916.root", "light_pt_150_250_eta_0_1p4_reweight"}},
    {"light_pt_250_inf_eta_0_1p4_190916", {"data/reweight_190916.root", "light_pt_250_inf_eta_0_1p4_reweight"}},
    {"light_pt_0_150_eta_1p4_2p5_190916", {"data/reweight_190916.root", "light_pt_0_150_eta_1p4_2p5_reweight"}},
    {"light_pt_150_250_eta_1p4_2p5_190916", {"data/reweight_190916.root", "light_pt_150_250_eta_1p4_2p5_reweight"}},
    {"light_pt_250_inf_eta_1p4_2p5_190916", {"data/reweight_190916.root", "light_pt_250_inf_eta_1p4_2p5_reweight"}}
  };

  std::string flav (const panda::Jet& jet) {

    return quickb::numB(jet) ? "heavy" : "light";

  }

  std::string pt (const panda::Jet& jet) {

    std::string pt_bin = "0_150";
    auto pt = jet.pt();
    if (pt > 150)
      pt_bin = "150_250";
    if (pt > 250)
      pt_bin = "250_inf";

    return "_pt_" + pt_bin;

  }

  std::string eta (const panda::Jet& jet) {

    std::string eta_bin = "0_1p4";
    auto eta = std::abs(jet.eta());
    if (eta > 1.4)
      eta_bin = "1p4_2p5";

    return "_eta_" + eta_bin;

  }

}


namespace quickb {

  double scale_b (const panda::Jet& jet) {

    return correctors.at(flav(jet)).GetCorrection(jet.deepCSVb);

  }

  double scale_b_pt (const panda::Jet& jet) {

    return correctors.at(flav(jet) + pt(jet)).
      GetCorrection(jet.deepCSVb);

  }

  double scale_b_pt_eta (const panda::Jet& jet) {

    return correctors.at(flav(jet) + pt(jet) + eta(jet)).
      GetCorrection(jet.deepCSVb);

  }

  double scale_b_pt_eta_v2 (const panda::Jet& jet) {

    return correctors.at(flav(jet) + pt(jet) + eta(jet) +
                         (input::ispuppi? "_190916" : "_190214")).
      GetCorrection(jet.deepCSVb);

  }

}


#endif
