#ifndef CROMBIE_METPHICORR_H
#define CROMBIE_METPHICORR_H


#include <string>
#include <utility>

#include <input.h>

#include "PandaTree/Objects/interface/Event.h"


namespace metphicorr {

  std::pair<double, double> METXYCorr_Met_MetPhi(double pt, double phi, double METxcorr, double METycorr){
  
    std::pair<double, double> TheXYCorr_Met_MetPhi(pt, phi);
    double CorrectedMET_x = pt * cos(phi) + METxcorr;
    double CorrectedMET_y = pt * sin(phi) + METycorr;
  
    double CorrectedMET = sqrt(CorrectedMET_x * CorrectedMET_x + CorrectedMET_y * CorrectedMET_y);
    double CorrectedMETPhi;

    if(CorrectedMET_x == 0 && CorrectedMET_y > 0)
      CorrectedMETPhi = TMath::Pi();

    else if(CorrectedMET_x == 0 && CorrectedMET_y < 0)
      CorrectedMETPhi = -TMath::Pi();

    else if(CorrectedMET_x > 0)
      CorrectedMETPhi = TMath::ATan(CorrectedMET_y/CorrectedMET_x);

    else if(CorrectedMET_x < 0 && CorrectedMET_y > 0)
      CorrectedMETPhi = TMath::ATan(CorrectedMET_y/CorrectedMET_x) + TMath::Pi();

    else if(CorrectedMET_x < 0 && CorrectedMET_y < 0)
      CorrectedMETPhi = TMath::ATan(CorrectedMET_y/CorrectedMET_x) - TMath::Pi();

    else
      CorrectedMETPhi = 0;

    TheXYCorr_Met_MetPhi.first = CorrectedMET;
    TheXYCorr_Met_MetPhi.second = CorrectedMETPhi;
    return TheXYCorr_Met_MetPhi;

  }

  void correctMETXY(const panda::Event& event, panda::RecoMet& met) {
    int npv = event.npv;
    int runnb = event.runNumber;

    // XY correction

    double METxcorr(0.), METycorr(0.);
    if (input::year == "2018"){
      if (event.isData){
        if(runnb >=315252 &&runnb<=316995) METxcorr = -(0.362865*npv -1.94505);
        if(runnb >=315252 &&runnb<=316995) METycorr = -(0.0709085*npv -0.307365);
        if(runnb >=316998 &&runnb<=319312) METxcorr = -(0.492083*npv -2.93552);
        if(runnb >=316998 &&runnb<=319312) METycorr = -(0.17874*npv -0.786844);
        if(runnb >=319313 &&runnb<=320393) METxcorr = -(0.521349*npv -1.44544);
        if(runnb >=319313 &&runnb<=320393) METycorr = -(0.118956*npv -1.96434);
        if(runnb >=320394 &&runnb<=325273) METxcorr = -(0.531151*npv -1.37568);
        if(runnb >=320394 &&runnb<=325273) METycorr = -(0.0884639*npv -1.57089);
      }
      else{
        METxcorr = -(0.296713*npv -0.141506);
        METycorr = -(0.115685*npv +0.0128193);
      }

      std::pair<double, double> nominal = METXYCorr_Met_MetPhi(met.pt, met.phi, METxcorr, METycorr);
      met.pt = nominal.first;
      met.phi = nominal.second;
      std::pair<double, double> up = METXYCorr_Met_MetPhi(met.ptCorrUp, met.phiCorrUp, METxcorr, METycorr);
      met.ptCorrUp = up.first;
      met.phiCorrUp = up.second;
      std::pair<double, double> down = METXYCorr_Met_MetPhi(met.ptCorrDown, met.phiCorrDown, METxcorr, METycorr);
      met.ptCorrDown = down.first;
      met.phiCorrDown = down.second;
    }
  }

}


#endif
