#ifndef DAN_BTAGREADERS_H
#define DAN_BTAGREADERS_H 1

#include <map>
#include <string>
#include <regex>
#include <sstream>

#include "BTagCalibrationStandalone.cpp" 

#include "PandaTree/Objects/interface/Event.h"

#include "TH2F.h"
#include "TH1D.h"
#include "TFile.h"

namespace {

  template<typename T>
  class PtEtaBins {
    using bin_type = unsigned;
    std::vector<T> internal;
    bin_type maxpt;
    bin_type maxeta;
  public:
    // Pass bins through these when setting
    void setmax_pteta (bin_type _pt, bin_type _eta) {
      maxpt = std::max(maxpt, _pt);
      maxeta = std::max(maxeta, _eta);
      internal.resize((maxpt + 1) * (maxeta + 1));
    }

    T& operator() (bin_type _pt, bin_type _eta) {
      return internal[std::min(maxpt, _pt) + (maxpt + 1) * std::min(maxeta, _eta)];
    }
  };

  auto cmva_hists = [] () {
    using binnedhists = PtEtaBins<std::map<std::string, TH1D>>;
    using filecontent = std::vector<binnedhists>;
    filecontent output(3);   // Three flavors

    auto default_flav = BTagEntry::FLAV_UDSG;
    std::regex expr {"(c_)?csv_ratio_Pt(\\d)_Eta(\\d)_final(_[\\w]+)?"};

    for (auto& flavor_file : {"data/cmva_rwt_fit_lf_v0_final_2017_3_29.root",
                              "data/cmva_rwt_fit_hf_v0_final_2017_3_29.root"}) {

      TFile infile {flavor_file};
      unsigned ptbin;
      unsigned etabin;
      std::cmatch matches;
      TH1D* ptr;

      auto loop_keys = [&] (std::function<void(binnedhists&)> func) {
        for (auto* key : *(infile.GetListOfKeys())) {
          std::stringstream buffer;

          if (std::regex_match(key->GetName(), matches, expr)) {
            ptr = static_cast<TH1D*>(static_cast<TKey*>(key)->ReadObj());
            if (not ptr->GetEntries())
              continue;
            buffer << matches[2] << " " << matches[3];
            buffer >> ptbin >> etabin;
            auto& binner = output[matches[1].length() ? BTagEntry::FLAV_C : default_flav];    // Flavor: default UDSG in first pass
            func(binner);
          }
        }
      };

      loop_keys([&] (binnedhists& binner) { binner.setmax_pteta(ptbin, etabin); });
      loop_keys([&] (binnedhists& binner) {
          binner(ptbin, etabin)
            [matches[4].length() ? std::string(matches[4]).substr(1) : "central"] = *ptr;   // systematic: strip leading "_"
        });
        

      // After first pass, change default entry to "b"
      default_flav = BTagEntry::FLAV_B;
    }

    return output;
  } ();

}

namespace btag {

  BTagEntry::JetFlavor flavor(const panda::Ref<panda::GenJet>& gen) {
    auto flavor = BTagEntry::FLAV_UDSG;
    if (gen.isValid()) {
      auto abspdgid = abs(gen->pdgid);
      if (abspdgid == 5)
        flavor = BTagEntry::FLAV_B;
      else if (abspdgid == 4)
        flavor = BTagEntry::FLAV_C;
    }
    return flavor;
  }

  // Just a floating value that defaults to 1.0
  class Scale {
  public:
    float operator=(float val) { _val = val; }
    float operator=(double val) { _val = val; }
    operator float() const { return _val; }
  private:
    float _val {1.0};
  };

  using ScaleWithSys = std::map<std::string, Scale>;

  // Just return all of the systematics at once
  ScaleWithSys get_scale(const panda::Jet& jet) {
    ScaleWithSys output;

    auto flav = flavor(jet.matchedGenJet);
    auto pt = jet.pt();
    auto eta = std::abs(jet.eta());

    /* auto& binner = cmva_hists[flav]; */

    unsigned ptbin;
    if (pt < 30)
      ptbin = 0;
    else if (pt < 40)
      ptbin = 1;
    else if (pt < 60)
      ptbin = 2;
    else if (pt < 100)
      ptbin = 3;
    else if (pt < 160)
      ptbin = 4;
    else
      ptbin = 5;

    unsigned etabin;
    if (eta < 0.8)
      etabin = 0;
    else if (eta < 1.6)
      etabin = 1;
    else
      etabin = 2;

    auto& hists = cmva_hists[flav](ptbin, etabin);

    for (auto& syshist : hists) {
      auto histbin = std::max(syshist.second.FindBin(jet.cmva), 1);
      auto content = syshist.second.GetBinContent(histbin);

      if (content)
        output[syshist.first] = content;
    }

    return output;
  }

}

#endif
