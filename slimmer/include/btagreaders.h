#ifndef DAN_BTAGREADERS_H
#define DAN_BTAGREADERS_H 1

#include <regex>

// #include "bcal/BTagCalibrationStandalone.cpp" 
#include "CondTools/BTau/interface/BTagCalibrationReader.h"

#include "PandaTree/Objects/interface/Event.h"

#include "crombie/EtaPhiMap.h"

#include "TH2F.h"
#include "TH1D.h"
#include "TFile.h"

#include "input.h"
#include "debugevent.h"

namespace {

  // Calibration reader backend

  /* static const BTagCalibration csv_calib {"csv", "data/CSVv2_Moriond17_B_H.csv"}; */
  static const BTagCalibration cmva_calib {"cmva", input::tagger == input::btagger::cmva ? "data/cMVAv2_Moriond17_B_H.csv" : "data/DeepCSV_94XSF_V2_B_F.csv"};
  static const BTagCalibration deepcsv_calib {"deepcsv", "data/DeepCSV_102XSF_V1.csv"};
  /* static const BTagCalibration deepcsv_calib {"deepcsv", "data/DeepCSV_94XSF_V2_B_F.csv"}; */

  // Here we define functions that returns the reader for each working point
  BTagCalibrationReader get_reader(const BTagEntry::OperatingPoint op, const BTagCalibration& calib) {
    if (op != BTagEntry::OP_RESHAPING) {
      BTagCalibrationReader reader(op, "central", {"up", "down"});
      reader.load(calib, BTagEntry::FLAV_B, "ttbar");
      reader.load(calib, BTagEntry::FLAV_C, "ttbar");
      reader.load(calib, BTagEntry::FLAV_UDSG, "incl");
      return reader;
    }

    BTagCalibrationReader reader(op, "central", {"up_jes", "down_jes", "up_lf", "down_lf", "up_hf", "down_hf",
          "up_hfstats1", "down_hfstats1", "up_hfstats2", "down_hfstats2",
          "up_lfstats1", "down_lfstats1", "up_lfstats2", "down_lfstats2",
          "up_cferr1", "down_cferr1", "up_cferr2", "down_cferr2"});

    for (auto flav : {BTagEntry::FLAV_B, BTagEntry::FLAV_C, BTagEntry::FLAV_UDSG})
      reader.load(calib, flav, "iterativefit");
    return reader;
  }

  auto effs = [] () {
    TFile infile {"data/btag_effs.root"};
    std::map<BTagEntry::JetFlavor, TH2F> output {
      {BTagEntry::FLAV_UDSG, *(static_cast<TH2F*>(infile.Get("lfeff")))},
      {BTagEntry::FLAV_C, *(static_cast<TH2F*>(infile.Get("ceff")))},
      {BTagEntry::FLAV_B, *(static_cast<TH2F*>(infile.Get("beff")))}
    };
    return output;
  } ();

  // Histogram-based backend

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
    bin_type get_maxpt () const { return maxpt; }
    bin_type get_maxeta () const { return maxeta; }

    T& operator() (bin_type _pt, bin_type _eta) {
      // Optimized away, but important to understand design
      /* return internal[std::min(maxpt, _pt) + (maxpt + 1) * std::min(maxeta, _eta)]; */
      return internal[_pt + (maxpt + 1) * _eta];
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

      // Loop through once to set the max for each binner
      loop_keys([&] (binnedhists& binner) { binner.setmax_pteta(ptbin, etabin); });
      // Loop through again to set values
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

  static EtaPhiMap<panda::GenJet> genjetmap {0.5, 3.0};

  BTagEntry::JetFlavor flavor(const panda::Jet& jet) {
    auto flavor = BTagEntry::FLAV_UDSG;
    for (auto& gen : btag::genjetmap.GetParticles(jet.eta(), jet.phi(), 0.3)) {
      auto abspdgid = abs(gen->pdgid);
      if (abspdgid == 5) {
        flavor = BTagEntry::FLAV_B;
        break;
      }
      else if (abspdgid == 4) {
        flavor = BTagEntry::FLAV_C;
        break;
      }
    }
    return flavor;
  }

  // Calibration reader front end

  using BCalReaders = std::map<const BTagEntry::OperatingPoint, const BTagCalibrationReader>;

  const BTagCalibrationReader cmva_reader = get_reader(BTagEntry::OP_RESHAPING, cmva_calib);
  const BTagCalibrationReader deepcsv_reader = get_reader(BTagEntry::OP_RESHAPING, deepcsv_calib);

  /* const BCalReaders cmva_readers = { */
  /*   {BTagEntry::OP_LOOSE, get_reader(BTagEntry::OP_LOOSE)}, */
  /*   {BTagEntry::OP_MEDIUM, get_reader(BTagEntry::OP_MEDIUM)}, */
  /*   {BTagEntry::OP_TIGHT, get_reader(BTagEntry::OP_TIGHT)} */
  /* }; */

  double eff(const double jetpt, const double jeteta, const BTagEntry::JetFlavor flav) {
    auto& hist = effs.at(flav);
    return hist.GetBinContent(hist.FindBin(std::min(jetpt, 950.0), std::abs(jeteta)));
  }

  // Histogram-based front end

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
  // Get the flavor ahead of time for use with other bits
  ScaleWithSys get_scale(const panda::Jet& jet, BTagEntry::JetFlavor flav) {
    ScaleWithSys output;

    auto pt = jet.pt();
    auto eta = std::abs(jet.eta());

    auto& binner = cmva_hists[flav];

    auto tagvar = input::tagger == input::btagger::cmva ? jet.cmva : jet.deepCSVb;

#define PTCASE(bin, cut)  \
  case bin: \
  if (pt >= cut) { \
    ptbin = bin; \
    break; \
  }
#define ETACASE(bin, cut)  \
  case bin: \
  if (eta >= cut) { \
    etabin = bin; \
    break; \
  }

    unsigned ptbin = 0;
    switch (binner.get_maxpt()) {
      PTCASE(5, 160);
      PTCASE(4, 100);
      PTCASE(3, 60);
      PTCASE(2, 40);
      PTCASE(1, 30);
    }

    unsigned etabin = 0;
    switch (binner.get_maxeta()) {
      ETACASE(2, 1.6);
      ETACASE(1, 0.8);
    }

    auto& hists = binner(ptbin, etabin);

    for (auto& syshist : hists) {
      auto histbin = std::max(syshist.second.FindBin(tagvar), 1);
      auto content = syshist.second.GetBinContent(histbin);

      if (content)
        output[syshist.first] = content;
    }

    if (debugevent::debug)
      std::cout << "Jet pt " << pt << " bin " << ptbin << " eta " << jet.eta() << " bin " << etabin
                << " tagvar " << tagvar
                << " matched " << jet.matchedGenJet.idx() << " flav " << flav
                << " central output " << output["central"] << std::endl;

    return output;
  }

}

#endif
