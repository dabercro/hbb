#ifndef JECCORRECTOR_H
#define JECCORRECTOR_H


#include <iostream>
#include <exception>

#include "TRandom3.h"

#include <boost/filesystem.hpp>

#include "PandaTree/Objects/interface/Event.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "JetMETCorrections/Modules/interface/JetResolution.h"


namespace panda {

  TRandom3 generator {};

  /**
     @brief A wrapper for CMSSW's FactorizedJetCorrector for Panda JetCollections
  */
  class JECCorrector {
  public:
    /**
       @param files_base Directory location and beginning of file name. e.g. "data/Autumn18_V3_MC"
       @param jet_type Type of jet to correct. e.g. "AK4PFchs"
    */
    JECCorrector (const std::string& files_base, const std::string& jet_type);

    /// Must be called at the beginning of each event to update the corrected values
    std::pair<panda::JetCollection, panda::RecoMet> get_jets_met (const panda::Event& event, const panda::JetCollection& jets, const RecoMet& met);

    const JetCollection& get_jets () const;
    const RecoMet& get_met () const;

    static void adjust_event (panda::Event& event, const std::string& year);

  private:

    /// Gets the smeared pT
    /* double smeared_pt (const panda::Jet& jet, const panda::Event& event); */

    /// Holds the corrected jets
    JetCollection m_corrected_jets {};

    /// Holds the corrected MET
    RecoMet m_corrected_met {};

    /// A vector of the JetCorrectorParameters, initialized by panda::JECCorrector::load_params
    std::vector<JetCorrectorParameters> m_corrector_params;
    /// The FactorizedJetCorrector that is used underneath
    FactorizedJetCorrector m_corrector;
    /* JME::JetResolution m_resolution; */

  };

}

namespace {

  // Used to initialize the JetCorrectionParameters
  std::vector<JetCorrectorParameters> load_params (const std::string& files_base, const std::string& jet_type) {

    std::vector<JetCorrectorParameters> output {};

    const std::vector<std::string> levels = {
      "L1FastJet",
      "L2Relative",
      "L3Absolute",
      "L2L3Residual"
    };

    for (const auto& level : levels) {

      const std::string correction_file = files_base + "_" + level + "_" + jet_type + ".txt";

      if (not boost::filesystem::exists(correction_file)) {
        std::cerr << "Cannot find correction file: " << correction_file << std::endl;
        throw std::runtime_error {"No file"};
      }

      std::cout << "Loading " << correction_file << std::endl;

      output.emplace_back(correction_file);
    }

    return output;

  }


  /* std::string uncertainty_file (const std::string& jet_type) { */
  /*   std::string base {"data/jec/Autumn18_V7_MC_PtResolution_"}; */
  /*   return  base + jet_type + ".txt"; */
  /* } */

}


panda::JECCorrector::JECCorrector (const std::string& files_base, const std::string& jet_type) :
  m_corrector_params {load_params(files_base, jet_type)},
  m_corrector {m_corrector_params} {}
  /* m_resolution {uncertainty_file(jet_type)} {} */


std::pair<panda::JetCollection, panda::RecoMet> panda::JECCorrector::get_jets_met (const panda::Event& event, const panda::JetCollection& jets, const RecoMet& met) {

  // Copy jets over
  m_corrected_jets = jets;
  m_corrected_met = met;

  TVector2 new_met {};
  new_met.SetMagPhi(met.pt, met.phi);

  TVector2 met_correction {};

  for (auto& jet : m_corrected_jets) {

    // FactorizedJetCorrectorCalculator is stupid and resets these every time
    m_corrector.setNPV(event.PV.npvs);
    m_corrector.setRho(event.fixedGridRhoFastjetAll);

    TLorentzVector jet_vec {};
    jet_vec.SetPtEtaPhiM((1 - jet.rawFactor) * jet.pt, jet.eta, jet.phi, jet.mass);

    m_corrector.setJetEta(jet_vec.Eta());
    m_corrector.setJetPt(jet_vec.Pt());
    m_corrector.setJetE(jet_vec.E());
    m_corrector.setJetPhi(jet_vec.Phi());
    m_corrector.setJetEMF(jet.chEmEF + jet.neEmEF); // Check that this is right
    m_corrector.setJetA(jet.area);

    auto scale = m_corrector.getCorrection();

    auto new_pt = scale * jet_vec.Pt();

    // Use minus new so that we don't have to flip phi.
    // This takes out the old correction and adds the new correction back in
    met_correction.SetMagPhi(jet_vec.Pt() - new_pt, jet_vec.Phi());

    new_met += met_correction;

    jet.pt = new_pt;

  }

  // Change stored MET
  m_corrected_met.pt = new_met.Mod();
  m_corrected_met.phi = new_met.Phi();

  return std::make_pair(m_corrected_jets, m_corrected_met);

}


const panda::JetCollection& panda::JECCorrector::get_jets () const {

  return m_corrected_jets;

}


const panda::RecoMet& panda::JECCorrector::get_met () const {

  return m_corrected_met;

}


void panda::JECCorrector::adjust_event (panda::Event& event, const std::string& year) {

  /* // No update needed for these */
  /* if (year == "2018_v6" or year == "2018_custom" or year == "2016") */
  /*   return; */

  int year_int = std::stoi(year);

  const std::map<int, std::string> begin = {
    {2016, "Summer16_07Aug2017"},
    {2017, "Fall17_17Nov2017"},
    {2018, "Autumn18"}
  };

  const std::map<int, std::string> vers = {
    {2016, (event.run == 1 ? std::string("_V11_MC")
            : (std::vector<std::string>{"BCD", "EF", "GH"}[(event.run > 276811) + (event.run > 278808)] + "_V11_DATA"))},
    {2017, (event.run == 1 ? std::string("_V32_MC")
            : (std::vector<std::string>{"A", "B", "C", "DE", "F"}[(event.run > 297019) + (event.run > 299329) + (event.run > 302029) + (event.run > 304826)] + "_V32_DATA"))},
    {2018, (event.run == 1 ? std::string("_V19_MC")
            : std::string("_Run") + (std::vector<std::string>{"A", "B", "C", "D"}[(event.run > 316995) + (event.run > 319312) + (event.run > 320393)] + "_V19_DATA"))}
  };

  static panda::JECCorrector corrector {std::string(getenv("CMSSW_BASE")) + "/data/jec/" + begin.at(year_int) + vers.at(year_int), "AK4PFchs"};

  auto [jets, met] = corrector.get_jets_met(event, event.Jet, event.MET);

  event.Jet = std::move(jets);
  event.MET = std::move(met);

}


#endif
