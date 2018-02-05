#ifndef DAN_BTAGREADERS_H
#define DAN_BTAGREADERS_H 1

#include <map>
#include <string>

#include "TLorentzVector.h"
#include "bcal/BTagCalibrationStandalone.h"

namespace {

  static const BTagCalibration csv_calib {"csv", "data/CSVv2_Moriond17_B_H.csv"};
  static const BTagCalibration cmva_calib {"cmva", "data/cMVAv2_Moriond17_B_H.csv"};

  // Here we define functions that returns the reader for each working point

  BTagCalibrationReader get_csv_reader(const BTagEntry::OperatingPoint op) {
    BTagCalibrationReader csv_reader(op, "central", {"up", "down"});
    csv_reader.load(csv_calib, BTagEntry::FLAV_B, "comb");
    csv_reader.load(csv_calib, BTagEntry::FLAV_C, "comb");
    csv_reader.load(csv_calib, BTagEntry::FLAV_UDSG, "incl");

    return csv_reader;
  }

  BTagCalibrationReader get_cmva_reader(const BTagEntry::OperatingPoint op) {
    BTagCalibrationReader cmva_reader(op, "central", {"up", "down"});
    cmva_reader.load(cmva_calib, BTagEntry::FLAV_B, "ttbar");
    cmva_reader.load(cmva_calib, BTagEntry::FLAV_C, "ttbar");
    cmva_reader.load(cmva_calib, BTagEntry::FLAV_UDSG, "incl");

    return cmva_reader;
  }

}

// Under here is the only stuff that should be need for the slimmer.

using BCalReaders = std::map<const BTagEntry::OperatingPoint, const BTagCalibrationReader>;

// Map operating points to the readers

/* const BCalReaders csv_readers = { */
/*   {BTagEntry::OP_LOOSE, get_csv_reader(BTagEntry::OP_LOOSE)}, */
/*   {BTagEntry::OP_MEDIUM, get_csv_reader(BTagEntry::OP_MEDIUM)}, */
/*   {BTagEntry::OP_TIGHT, get_csv_reader(BTagEntry::OP_TIGHT)} */
/* }; */

const BCalReaders cmva_readers = {
  {BTagEntry::OP_LOOSE, get_cmva_reader(BTagEntry::OP_LOOSE)},
  {BTagEntry::OP_MEDIUM, get_cmva_reader(BTagEntry::OP_MEDIUM)},
  {BTagEntry::OP_TIGHT, get_cmva_reader(BTagEntry::OP_TIGHT)}
};

#endif
