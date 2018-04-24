namespace {
  auto hist = [] () {
    TFile infile {"data/puid.root"};
    return *(static_cast<TH2D*>(infile.Get("puid_76x_loose")));
  } ();
}

namespace puid {

  // Does the jet pass the low PUID working point?
  bool loose(const panda::Jet& jet) {
    return jet.puid > hist.GetBinContent(hist.GetBin(std::abs(jet.eta()),
                                                     std::min(jet.pt(), 39.0)));
  }

}
