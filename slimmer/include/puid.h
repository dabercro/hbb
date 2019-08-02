// Needs headers, update function is weird too

namespace {
  auto hist = [] () {
    TFile infile {"data/puid.root"};
    return *(static_cast<TH2D*>(infile.Get("puid_76x_loose")));
  } ();
}

class hbbfile;

namespace puid {

  void update(const panda::Jet& jet, hbbfile* output);

  // Does the jet pass the low PUID working point?
  bool loose(const panda::Jet& jet, hbbfile* output = nullptr) {
    if (output)
      update(jet, output);

    auto to_pass = hist.GetBinContent(hist.FindBin(std::abs(jet.eta()), std::min(jet.pt(), 39.0)));
    if (debugevent::debug)
      std::cout << "Jet with pt " << jet.pt() << " and puid " << jet.puid << " needs to pass " << to_pass << std::endl;
    return jet.puid > to_pass;
  }

  /* bool loose(const panda::Jet& jet) { */

  /*   // Working points for PUID, highpt is pt > 30 GeV */
  /*   using workingpoints = std::vector<float>; */
  /*   static const workingpoints highpt = {-0.89, -0.52, -0.38, -0.30}; */
  /*   static const workingpoints lowpt = {-0.97, -0.68, -0.53, -0.47}; */

  /*   auto abseta = std::abs(jet.eta()); */
  /*   int index = abseta < 2.75 ? (abseta < 2.5 ? 0 : 1) : (abseta < 3.0 ? 2 : 3); */

  /*   auto to_pass = (jet.pt() < 30 ? lowpt : highpt)[index]; */
  /*   if (debugevent::debug) */
  /*     std::cout << "Jet with pt " << jet.pt() << " and puid " << jet.puid << " needs to pass " << to_pass << std::endl; */

  /*   return jet.puid > to_pass; */

  /* } */

}
