#ifndef TRIGGERS_H
#define TRIGGERS_H


#include <exception>
#include <vector>

#include "PandaTree/Objects/interface/Event.h"


namespace triggers {

  using tokens = std::vector<unsigned>;

  enum class trigger {
    MET,
    HBB_2016,
    OVERLAP
  };

}

namespace {

  panda::Event* event_ptr {nullptr};

  const std::map<triggers::trigger, const std::vector<const char*>> paths = {
    {triggers::trigger::MET, {
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
      }},
    {triggers::trigger::HBB_2016, {
        "HLT_PFMETNoMu110_PFMHTNoMu110_IDTight",
        "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight",
        "HLT_PFMET170_NoiseCleaned",
        "HLT_PFMET170_HBHECleaned",
        "HLT_PFMET170_HBHE_BeamHaloCleaned"
      }},
    {triggers::trigger::OVERLAP, {
        "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"
      }}};

  triggers::tokens get_tokens (const std::vector<const char*>& paths) {
    triggers::tokens output;
    for (auto path : paths)
      output.push_back(event_ptr->registerTrigger(path));
    return output;
  }

  std::map<triggers::trigger, triggers::tokens> trigger_tokens {};

}


namespace triggers {

  void init (panda::Event& event) {
    event_ptr = &event;
    trigger_tokens.clear();
    for (auto& type : paths)
      trigger_tokens[type.first] = get_tokens(type.second);
  }

  bool check (trigger trigger_type) {
    if (not event_ptr)
      throw std::logic_error{"Triggers not initialized!"};
    for (auto token : trigger_tokens.at(trigger_type)) {
      if (event_ptr->triggerFired(token))
        return true;
    }
    return false;
  };

}

#endif
