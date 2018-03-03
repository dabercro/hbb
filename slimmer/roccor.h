#include "roccor/RoccoR.cc" // https://twiki.cern.ch/twiki/bin/view/CMS/RochcorMuon

namespace {
  RoccoR rochester {"roccor/rcdata.2016.v3"};
}

namespace roccor {
  double scale(panda::Event& event, panda::Muon& muon) {
    return 1.0;
  }
}
