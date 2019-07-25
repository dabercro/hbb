#ifndef TRIGGERS_H
#define TRIGGERS_H


#include <exception>
#include <vector>

#include "PandaTree/Objects/interface/Event.h"


namespace triggers {

  using tokens = std::vector<unsigned>;

}

namespace {

  panda::Event* event_ptr {nullptr};

  const std::vector<const char*> met_paths = {
    "HLT_PFMET170_NoiseCleaned",
    "HLT_PFMET170_HBHECleaned",
    "HLT_PFMET170_JetIdCleaned",
    "HLT_PFMET170_NotCleaned",
    "HLT_PFMET170_HBHE_BeamHaloCleaned",
    "HLT_PFMETNoMu120_NoiseCleaned_PFMHTNoMu120_IDTight",
    "HLT_PFMETNoMu110_NoiseCleaned_PFMHTNoMu110_IDTight",
    "HLT_PFMETNoMu90_NoiseCleaned_PFMHTNoMu90_IDTight",
    "HLT_PFMETNoMu90_PFMHTNoMu90_IDTight",
    "HLT_PFMETNoMu100_PFMHTNoMu100_IDTight_PFHT60",
    "HLT_PFMETNoMu100_PFMHTNoMu100_IDTight",
    "HLT_PFMETNoMu110_PFMHTNoMu110_IDTight",
    "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight",
    "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60",
    "HLT_PFMETNoMu130_PFMHTNoMu130_IDTight",
    "HLT_PFMETNoMu140_PFMHTNoMu140_IDTight"
  };

  const std::vector<const char*> hbb_2016_paths = {
    "HLT_PFMETNoMu110_PFMHTNoMu110_IDTight",
    "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight",
    "HLT_PFMET170_NoiseCleaned",
    "HLT_PFMET170_HBHECleaned",
    "HLT_PFMET170_HBHE_BeamHaloCleaned"
  };

  const std::vector<const char*> overlap_paths = {
    "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"
  };

  triggers::tokens get_tokens (const std::vector<const char*>& paths) {
    triggers::tokens output;
    for (auto path : paths)
      output.push_back(event_ptr->registerTrigger(path));
    return output;
  }

}


namespace triggers {

  tokens met_tokens {};
  tokens hbb_2016_tokens {};
  tokens overlap_tokens {};

  void init (panda::Event& event) {
    event_ptr = &event;
    met_tokens = get_tokens(met_paths);
    hbb_2016_tokens = get_tokens(hbb_2016_paths);
    overlap_tokens = get_tokens(overlap_paths);
  }

  bool check_tokens (tokens& trigger_tokens) {
    if (not event_ptr)
      throw std::logic_error{"Triggers not initialized!"};
    for (auto token : trigger_tokens) {
      if (event_ptr->triggerFired(token))
        return true;
    }
    return false;
  };

}

#endif
