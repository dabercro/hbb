#ifndef DAN_BTAGREADERS_H
#define DAN_BTAGREADERS_H 1

#include <map>
#include <string>

#include "TLorentzVector.h"
#include "BTagCalibrationStandalone.cpp" 

#include "PandaTree/Objects/interface/Event.h"

namespace {

  /* static const BTagCalibration csv_calib {"csv", "data/CSVv2_Moriond17_B_H.csv"}; */
  static const BTagCalibration cmva_calib {"cmva", "data/cMVAv2_Moriond17_B_H.csv"};

  // Here we define functions that returns the reader for each working point
  BTagCalibrationReader get_cmva_reader(const BTagEntry::OperatingPoint op) {
    if (op != BTagEntry::OP_RESHAPING) {
      BTagCalibrationReader cmva_reader(op, "central", {"up", "down"});
      cmva_reader.load(cmva_calib, BTagEntry::FLAV_B, "ttbar");
      cmva_reader.load(cmva_calib, BTagEntry::FLAV_C, "ttbar");
      cmva_reader.load(cmva_calib, BTagEntry::FLAV_UDSG, "incl");
      return cmva_reader;
    }

    BTagCalibrationReader cmva_reader(op, "central", {"up_jes", "down_jes", "up_lf", "down_lf", "up_hf", "down_hf",
          "up_hfstats1", "down_hfstats1", "up_hfstats2", "down_hfstats2",
          "up_lfstats1", "down_lfstats1", "up_lfstats2", "down_lfstats2",
          "up_cferr1", "down_cferr1", "up_cferr2", "down_cferr2"});

    for (auto flav : {BTagEntry::FLAV_B, BTagEntry::FLAV_C, BTagEntry::FLAV_UDSG})
      cmva_reader.load(cmva_calib, flav, "iterativefit");
    return cmva_reader;
  }
}

namespace btag {
  using BCalReaders = std::map<const BTagEntry::OperatingPoint, const BTagCalibrationReader>;

  const BTagCalibrationReader cmva_reader = get_cmva_reader(BTagEntry::OP_RESHAPING);

  const BCalReaders cmva_readers = {
    {BTagEntry::OP_LOOSE, get_cmva_reader(BTagEntry::OP_LOOSE)},
    {BTagEntry::OP_MEDIUM, get_cmva_reader(BTagEntry::OP_MEDIUM)},
    {BTagEntry::OP_TIGHT, get_cmva_reader(BTagEntry::OP_TIGHT)}
  };

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

}

#endif
