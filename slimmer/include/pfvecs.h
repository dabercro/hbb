#ifndef PFVECS_H
#define PFVECS_H

#include <vector>

#include "PandaTree/Objects/interface/Event.h"

namespace pfvecs {

  class PFVecs {
  public:
    PFVecs (unsigned size) :
      pf_pt_sorted_pt (size),
      pf_pt_sorted_eta (size),
      pf_pt_sorted_phi (size),
      pf_pt_sorted_m (size),
      pf_fastjet_sorted_pt (size),
      pf_fastjet_sorted_eta (size),
      pf_fastjet_sorted_phi (size),
      pf_fastjet_sorted_m (size) {}

    std::vector<float> pf_pt_sorted_pt;
    std::vector<float> pf_pt_sorted_eta;
    std::vector<float> pf_pt_sorted_phi;
    std::vector<float> pf_pt_sorted_m;

    std::vector<float> pf_fastjet_sorted_pt;
    std::vector<float> pf_fastjet_sorted_eta;
    std::vector<float> pf_fastjet_sorted_phi;
    std::vector<float> pf_fastjet_sorted_m;
  };

  PFVecs get_vecs(const panda::Jet& jet) {
    PFVecs output {jet.constituents.size()};

    return output;
  }

}

#endif
