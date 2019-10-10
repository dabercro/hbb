#ifndef JECCORRECTOR_H
#define JECCORRECTOR_H


#include <iostream>
#include <exception>

#include <boost/filesystem.hpp>

#include "myrandom.h"

#include "PandaTree/Objects/interface/Event.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "JetMETCorrections/Modules/interface/JetResolution.h"


namespace panda {
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
    void update_event (const panda::Event& event, const panda::JetCollection& jets, const RecoMet& met);

    const JetCollection& get_jets () const;
    const RecoMet& get_met () const;

  private:

    /// Gets the smeared pT
    double smeared_pt (const panda::Jet& jet, const panda::Event& event);

    /// Holds the corrected jets
    JetCollection m_corrected_jets {};

    /// Holds the corrected MET
    RecoMet m_corrected_met {};

    /// A vector of the JetCorrectorParameters, initialized by panda::JECCorrector::load_params
    std::vector<JetCorrectorParameters> m_corrector_params;
    /// The FactorizedJetCorrector that is used underneath
    FactorizedJetCorrector m_corrector;
    JME::JetResolution m_resolution;

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

      output.emplace_back(correction_file);
    }

    return output;

  }


  std::string uncertainty_file (const std::string& jet_type) {
    std::string base {"data/jec/Autumn18_V7_MC_PtResolution_"};
    return  base + jet_type + ".txt";
  }

}


panda::JECCorrector::JECCorrector (const std::string& files_base, const std::string& jet_type) :
  m_corrector_params {load_params(files_base, jet_type)},
  m_corrector {m_corrector_params},
  m_resolution {uncertainty_file(jet_type)} {}


void panda::JECCorrector::update_event (const panda::Event& event, const panda::JetCollection& jets, const RecoMet& met) {

  // Copy jets over
  m_corrected_jets = jets;
  m_corrected_met = met;

  TVector2 new_met {met.v()};
  TVector2 met_correction {};

  for (auto& jet : m_corrected_jets) {

    // FactorizedJetCorrectorCalculator is stupid and resets these every time
    m_corrector.setNPV(event.npv);
    m_corrector.setRho(event.rho);

    m_corrector.setJetEta(jet.eta());
    m_corrector.setJetPt(jet.rawPt);
    m_corrector.setJetE(jet.e());
    m_corrector.setJetPhi(jet.phi());
    m_corrector.setJetEMF(jet.cef + jet.nef); // Check that this is right
    m_corrector.setJetA(jet.area);

    auto scale = m_corrector.getCorrection();

    auto new_pt = scale * jet.rawPt;

    // Use minus new so that we don't have to flip phi.
    // This takes out the old correction and adds the new correction back in
    met_correction.SetMagPhi(jet.pt() - new_pt, jet.phi());

    new_met += met_correction;

    jet.setPtEtaPhiM(new_pt, jet.eta(), jet.phi(), jet.m());
    if (not event.isData)
      jet.ptSmear = smeared_pt(jet, event);

  }

  m_corrected_jets.sort(panda::Particle::PtGreater);

  // Change stored MET
  m_corrected_met.setXY(new_met.X(), new_met.Y());

}


const panda::JetCollection& panda::JECCorrector::get_jets () const {

  return m_corrected_jets;

}


const panda::RecoMet& panda::JECCorrector::get_met () const {

  return m_corrected_met;

}


double panda::JECCorrector::smeared_pt (const panda::Jet& jet, const panda::Event& event) {

  // Get the smearing resolution
  JME::JetParameters jet_params {};

  jet_params.setJetArea(jet.area);
  jet_params.setJetE(jet.e());
  jet_params.setJetPt(jet.pt());
  jet_params.setJetEta(jet.eta());

  /*
  Pretty sure I don't need these:

  JetParameters & setMu (float mu);
  */

  jet_params.setNPV(event.npv);
  jet_params.setRho(event.rho);

  auto resolution = m_resolution.getResolution(jet_params);

  auto& gen = jet.matchedGenJet;
  double pt = (gen.isValid() ? gen->pt() : jet.pt());

  return jet.pt() + myrandom::gen.Gaus(0, resolution) * pt;

}


#endif
