#ifndef ELECTRONID_H
#define ELECTRONID_H 1


#include <array>

#include "TMath.h"
#include "TMVA/Reader.h"
#include "TMVA/IMethod.h"

namespace {
  const auto names = {"ele_eta", "ele_ecalIso/ele_pt", "ele_hcalIso/ele_pt",
                      "ele_trackIso/ele_pt", "ele_chIso/ele_pt", "ele_nhIso/ele_pt",
                      "ele_phIso/ele_pt", "ele_dxy", "ele_dz", "ele_sieie", "ele_sipip",
                      "ele_r9", "ele_dEtaInSeed", "ele_dPhiIn", "ele_hOverE",
                      "ele_ooEmooP", "ele_nMissingHits", "ele_tripleCharge"};

  std::array<Float_t, 18> vars;
  static TMVA::Reader reader {"Silent"};
  auto* method = [] () {
    unsigned index = 0;
    for (auto name : names)
      reader.AddVariable(name, &vars[index++]);

    return reader.BookMVA("electronMVA", "data/hfEleMva_hfEleMva_hp0_test4.weights.xml");
  } ();
}

namespace electronid {

  // Check if the electron inside the jet is good
  bool is_good(const panda::Electron& ele, const panda::Jet& jet) {

    float track_pt = ele.trackP/TMath::CosH(ele.eta());

    vars[0] = ele.eta();
    vars[1] = ele.ecalIso/track_pt;
    vars[2] = ele.hcalIso/track_pt;
    vars[3] = ele.trackIso/track_pt;
    vars[4] = ele.chIso/track_pt;
    vars[5] = ele.nhIso/track_pt;
    vars[6] = ele.phIso/track_pt;
    vars[7] = ele.dxy;
    vars[8] = ele.dz;
    vars[9] = ele.sieie;
    vars[10] = ele.sipip;
    vars[11] = ele.r9;
    vars[12] = ele.dEtaInSeed;
    vars[13] = ele.dPhiIn;
    vars[14] = ele.hOverE;
    vars[15] = std::abs(1.0 - ele.eseed/std::max(0.0001f, ele.trackP))/ele.ecalE;
    vars[16] = ele.nMissingHits;
    vars[17] = ele.tripleCharge;

    return method->GetMvaValue() > 0;
  }

}


#endif
