#ifndef DAN_CORR_H
#define DAN_CORR_H

#include "TF1.h"

namespace {
  auto hists = [] () {
    std::vector<TF1> output;
    TFile infile {"data/puppiCorr.root"};
    // First member is GEN
    // Second is central
    // Third is forward correction
    for (auto name : {"puppiJECcorr_gen", "puppiJECcorr_reco_0eta1v3", "puppiJECcorr_reco_1v3eta2v5"})
      output.push_back(*(static_cast<TF1*>(infile.Get(name))));

    return output;
  } ();
}

namespace corrections {
  double mSD(float pt, float eta) {
    // All are corrected by GEN, decided central or forward with eta
    return hists[0].Eval(pt) * hists[(std::abs(eta) <= 1.3 ? 1 : 2)].Eval(pt);
  }
}

#endif
