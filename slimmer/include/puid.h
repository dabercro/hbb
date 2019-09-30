#ifndef PUID_H
#define PUID_H 1

#include "input.h"

#include "TFile.h"
#include "TH2D.h"

namespace {
  auto hist = [] () {
    TFile infile {"data/puid.root"};
    return *(static_cast<TH2D*>(infile.Get("puid_76x_loose")));
  } ();
}

namespace puid {

  // Does the jet pass the low PUID working point?
  bool loose(const panda::Jet& jet) {

    if (input::ispuppi)
      return true;

    auto to_pass = hist.GetBinContent(hist.FindBin(std::abs(jet.eta()), std::min(jet.pt(), 39.0)));
    if (debugevent::debug)
      std::cout << "Jet with pt " << jet.pt() << " and puid " << jet.puid << " needs to pass " << to_pass << std::endl;
    return jet.puid > to_pass;

  }

}

#endif
