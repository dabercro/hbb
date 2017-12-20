#ifndef DAN_BTAGREADERS_H
#define DAN_BTAGREADERS_H 1

#include <map>
#include <string>

#include "bcal/BCalStrings.h"
#include "bcal/BTagCalibrationStandalone.h"

// Get the calibrations from hard-coded strings
// This way, we don't have to carry .csv files around with us
// These strings are in bcal/BCalStrings.h, generated by "crombie bcalstrings"

namespace {

  BTagCalibration get_calib(const std::string tagger, const std::string& csv_string) {
    BTagCalibration calib(tagger);
    calib.readCSV(csv_string);
    return calib;
  }

  static auto csv_calib = get_calib("csv", bcalstrings::csv);
  static auto cmva_calib = get_calib("cmva", bcalstrings::cmva);

  // Here we define functions that returns the reader for each working point

  BTagCalibrationReader get_csv_reader(const BTagEntry::OperatingPoint op) {
    BTagCalibrationReader csv_reader(op, "central", {"up", "down"});
    csv_reader.load(csv_calib, BTagEntry::FLAV_B, "comb");
    // csv_reader.load(csv_calib, BTagEntry::FLAV_C, "comb");
    csv_reader.load(csv_calib, BTagEntry::FLAV_UDSG, "incl");

    return csv_reader;
  }

  BTagCalibrationReader get_cmva_reader(const BTagEntry::OperatingPoint op) {

    // Setup the B-tag calibration readers
    BTagCalibrationReader cmva_reader(op, "central", {"up", "down"});
    cmva_reader.load(cmva_calib, BTagEntry::FLAV_B, "ttbar");

    return cmva_reader;
  }

}

using BCalReaders = std::map<const BTagEntry::OperatingPoint, const BTagCalibrationReader>;

const BCalReaders csv_readers = {
  {BTagEntry::OP_LOOSE, get_csv_reader(BTagEntry::OP_LOOSE)},
  {BTagEntry::OP_MEDIUM, get_csv_reader(BTagEntry::OP_MEDIUM)},
  {BTagEntry::OP_TIGHT, get_csv_reader(BTagEntry::OP_TIGHT)}
};

const BCalReaders cmva_readers = {
  {BTagEntry::OP_LOOSE, get_cmva_reader(BTagEntry::OP_LOOSE)},
  {BTagEntry::OP_MEDIUM, get_cmva_reader(BTagEntry::OP_MEDIUM)},
  {BTagEntry::OP_TIGHT, get_cmva_reader(BTagEntry::OP_TIGHT)}
};

#endif
