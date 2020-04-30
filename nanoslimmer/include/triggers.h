#ifndef TRIGGERS_H
#define TRIGGERS_H

#include <string>

#include "PandaTree/Objects/interface/Event.h"

namespace triggers {

  struct triggers {
    bool met;
    bool single_ele;
    bool single_mu;
    bool double_ele;
    bool double_mu;
  };

  triggers fired (const panda::Event& e, const std::string& year) {

    // Pick which function is called, based on year

    if (year == "2016")
      return triggers {
        e.HLT.PFMET110_PFMHT110_IDTight or e.HLT.PFMET120_PFMHT120_IDTight or e.HLT.PFMET170_NoiseCleaned or e.HLT.PFMET170_BeamHaloCleaned or e.HLT.PFMET170_HBHECleaned,
        e.HLT.Ele27_WPTight_Gsf,
        e.HLT.IsoMu24 or e.HLT.IsoTkMu24,
        e.HLT.Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ,
        e.HLT.Mu17_TrkIsoVVL_Mu8_TrkIsoVVL or e.HLT.Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL or e.HLT.Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ or e.HLT.Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ
      };

    if (year == "2017")
      return triggers {
        e.HLT.PFMET120_PFMHT120_IDTight or e.HLT.PFMET120_PFMHT120_IDTight_PFHT60,
        e.HLT.Ele32_WPTight_Gsf_L1DoubleEG,
        e.HLT.IsoMu27,
        e.HLT.Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ,
        e.HLT.Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8 or e.HLT.Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_Mass8
      };

    if (year == "2018")
      return triggers {
        e.HLT.PFMET120_PFMHT120_IDTight,
        e.HLT.Ele32_WPTight_Gsf,
        e.HLT.IsoMu24,
        e.HLT.Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ,
        e.HLT.Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8
      };

  }

}

#endif
