namespace puid {

  // Does the jet pass the low PUID working point?
  bool loose(const panda::Jet& jet) {

    // Working points for PUID, highpt is pt > 30 GeV
    using workingpoints = std::vector<float>;
    static const workingpoints highpt = {-0.89, -0.52, -0.38, -0.30};
    static const workingpoints lowpt = {-0.97, -0.68, -0.53, -0.47};

    auto abseta = std::abs(jet.eta());
    int index = abseta < 2.75 ? (abseta < 2.5 ? 0 : 1) : (abseta < 3.0 ? 2 : 3);

    return jet.puid > (jet.pt() < 30 ? lowpt : highpt)[index];

  }

}
