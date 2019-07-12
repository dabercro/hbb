#ifndef CROMBIE_REGFILE_H
#define CROMBIE_REGFILE_H

#include <string>
#include <vector>
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "lazytf.h"
#include "genjet.h"
#include "pfvecs.h"
#include "regression.h"
#include "TVector2.h"

class regfile {

 public:
  regfile(const char* outfile_name, const char* name = "events");
  ~regfile() { write(t); f->Close(); }

  Float_t Jet_btagCMVA;
  Float_t Jet_btagDeepB;
  Float_t Jet_btagDeepC;
  Float_t Jet_chEmEF;
  Float_t Jet_chHEF;
  Float_t Jet_e;
  Float_t Jet_energyRing_dR0_ch;
  Float_t Jet_energyRing_dR0_ch_Jet_rawEnergy;
  Float_t Jet_energyRing_dR0_em;
  Float_t Jet_energyRing_dR0_em_Jet_rawEnergy;
  Float_t Jet_energyRing_dR0_mu;
  Float_t Jet_energyRing_dR0_mu_Jet_rawEnergy;
  Float_t Jet_energyRing_dR0_neut;
  Float_t Jet_energyRing_dR0_neut_Jet_rawEnergy;
  Float_t Jet_energyRing_dR1_ch;
  Float_t Jet_energyRing_dR1_ch_Jet_rawEnergy;
  Float_t Jet_energyRing_dR1_em;
  Float_t Jet_energyRing_dR1_em_Jet_rawEnergy;
  Float_t Jet_energyRing_dR1_mu;
  Float_t Jet_energyRing_dR1_mu_Jet_rawEnergy;
  Float_t Jet_energyRing_dR1_neut;
  Float_t Jet_energyRing_dR1_neut_Jet_rawEnergy;
  Float_t Jet_energyRing_dR2_ch;
  Float_t Jet_energyRing_dR2_ch_Jet_rawEnergy;
  Float_t Jet_energyRing_dR2_em;
  Float_t Jet_energyRing_dR2_em_Jet_rawEnergy;
  Float_t Jet_energyRing_dR2_mu;
  Float_t Jet_energyRing_dR2_mu_Jet_rawEnergy;
  Float_t Jet_energyRing_dR2_neut;
  Float_t Jet_energyRing_dR2_neut_Jet_rawEnergy;
  Float_t Jet_energyRing_dR3_ch;
  Float_t Jet_energyRing_dR3_ch_Jet_rawEnergy;
  Float_t Jet_energyRing_dR3_em;
  Float_t Jet_energyRing_dR3_em_Jet_rawEnergy;
  Float_t Jet_energyRing_dR3_mu;
  Float_t Jet_energyRing_dR3_mu_Jet_rawEnergy;
  Float_t Jet_energyRing_dR3_neut;
  Float_t Jet_energyRing_dR3_neut_Jet_rawEnergy;
  Float_t Jet_energyRing_dR4_ch;
  Float_t Jet_energyRing_dR4_ch_Jet_rawEnergy;
  Float_t Jet_energyRing_dR4_em;
  Float_t Jet_energyRing_dR4_em_Jet_rawEnergy;
  Float_t Jet_energyRing_dR4_mu;
  Float_t Jet_energyRing_dR4_mu_Jet_rawEnergy;
  Float_t Jet_energyRing_dR4_neut;
  Float_t Jet_energyRing_dR4_neut_Jet_rawEnergy;
  Float_t Jet_energyRing_dR5_ch;
  Float_t Jet_energyRing_dR5_ch_Jet_rawEnergy;
  Float_t Jet_energyRing_dR5_em;
  Float_t Jet_energyRing_dR5_em_Jet_rawEnergy;
  Float_t Jet_energyRing_dR5_mu;
  Float_t Jet_energyRing_dR5_mu_Jet_rawEnergy;
  Float_t Jet_energyRing_dR5_neut;
  Float_t Jet_energyRing_dR5_neut_Jet_rawEnergy;
  Float_t Jet_eta;
  Float_t Jet_leadTrackPt;
  Float_t Jet_leptonDeltaR;
  Int_t Jet_leptonPdgId;
  Float_t Jet_leptonPt;
  Float_t Jet_leptonPtRel;
  Float_t Jet_leptonPtRelInv;
  Float_t Jet_mass;
  Float_t Jet_mcEta;
  Float_t Jet_mcFlavour;
  Float_t Jet_mcM;
  Float_t Jet_mcPhi;
  Float_t Jet_mcPt;
  Float_t Jet_met_dPhi;
  Float_t Jet_mt;
  Float_t Jet_neEmEF;
  Float_t Jet_neHEF;
  UShort_t Jet_numDaughters_pt03;
  UShort_t Jet_numberOfDaughters;
  Float_t Jet_pf_0_deta;
  Float_t Jet_pf_0_dphi;
  Float_t Jet_pf_0_dxy;
  Float_t Jet_pf_0_dz;
  Int_t Jet_pf_0_pdgid;
  Float_t Jet_pf_0_ptfrac;
  Float_t Jet_pf_0_puppiwt;
  Float_t Jet_pf_1_deta;
  Float_t Jet_pf_1_dphi;
  Float_t Jet_pf_1_dxy;
  Float_t Jet_pf_1_dz;
  Int_t Jet_pf_1_pdgid;
  Float_t Jet_pf_1_ptfrac;
  Float_t Jet_pf_1_puppiwt;
  Float_t Jet_pf_2_deta;
  Float_t Jet_pf_2_dphi;
  Float_t Jet_pf_2_dxy;
  Float_t Jet_pf_2_dz;
  Int_t Jet_pf_2_pdgid;
  Float_t Jet_pf_2_ptfrac;
  Float_t Jet_pf_2_puppiwt;
  Float_t Jet_pf_3_deta;
  Float_t Jet_pf_3_dphi;
  Float_t Jet_pf_3_dxy;
  Float_t Jet_pf_3_dz;
  Int_t Jet_pf_3_pdgid;
  Float_t Jet_pf_3_ptfrac;
  Float_t Jet_pf_3_puppiwt;
  Float_t Jet_pf_4_deta;
  Float_t Jet_pf_4_dphi;
  Float_t Jet_pf_4_dxy;
  Float_t Jet_pf_4_dz;
  Int_t Jet_pf_4_pdgid;
  Float_t Jet_pf_4_ptfrac;
  Float_t Jet_pf_4_puppiwt;
  std::vector<Float_t> Jet_pf_fastjet_sorted_eta;
  std::vector<Float_t> Jet_pf_fastjet_sorted_m;
  std::vector<Float_t> Jet_pf_fastjet_sorted_phi;
  std::vector<Float_t> Jet_pf_fastjet_sorted_pt;
  std::vector<Float_t> Jet_pf_pt_sorted_eta;
  std::vector<Float_t> Jet_pf_pt_sorted_m;
  std::vector<Float_t> Jet_pf_pt_sorted_phi;
  std::vector<Float_t> Jet_pf_pt_sorted_pt;
  Float_t Jet_phi;
  Float_t Jet_pt;
  Float_t Jet_ptd;
  Float_t Jet_puId;
  Float_t Jet_puppi_charged_deta;
  Float_t Jet_puppi_charged_dphi;
  Float_t Jet_puppi_charged_e;
  Float_t Jet_puppi_charged_eta;
  Float_t Jet_puppi_charged_m;
  Float_t Jet_puppi_charged_phi;
  Float_t Jet_puppi_charged_pt;
  Float_t Jet_puppi_charged_ptfrac;
  Float_t Jet_puppi_charged_pu_deta;
  Float_t Jet_puppi_charged_pu_dphi;
  Float_t Jet_puppi_charged_pu_e;
  Float_t Jet_puppi_charged_pu_eta;
  Float_t Jet_puppi_charged_pu_m;
  Float_t Jet_puppi_charged_pu_phi;
  Float_t Jet_puppi_charged_pu_pt;
  Float_t Jet_puppi_charged_pu_ptfrac;
  Float_t Jet_puppi_neutral_deta;
  Float_t Jet_puppi_neutral_dphi;
  Float_t Jet_puppi_neutral_e;
  Float_t Jet_puppi_neutral_eta;
  Float_t Jet_puppi_neutral_m;
  Float_t Jet_puppi_neutral_phi;
  Float_t Jet_puppi_neutral_pt;
  Float_t Jet_puppi_neutral_ptfrac;
  Float_t Jet_puppi_neutral_pu_deta;
  Float_t Jet_puppi_neutral_pu_dphi;
  Float_t Jet_puppi_neutral_pu_e;
  Float_t Jet_puppi_neutral_pu_eta;
  Float_t Jet_puppi_neutral_pu_m;
  Float_t Jet_puppi_neutral_pu_phi;
  Float_t Jet_puppi_neutral_pu_pt;
  Float_t Jet_puppi_neutral_pu_ptfrac;
  Float_t Jet_raw;
  Float_t Jet_rawEnergy;
  Float_t Jet_rawFactor;
  std::vector<float> Jet_tf_190711_0;
  Float_t Jet_tf_190711_0_deta;
  Float_t Jet_tf_190711_0_dphi;
  Float_t Jet_tf_190711_0_mratio;
  Float_t Jet_tf_190711_0_ptratio;
  std::vector<float> Jet_tf_190711_2;
  Float_t Jet_tf_190711_2_deta;
  Float_t Jet_tf_190711_2_dphi;
  Float_t Jet_vtx3dL;
  Float_t Jet_vtx3deL;
  Float_t Jet_vtxMass;
  Short_t Jet_vtxNtrk;
  Float_t Jet_vtxPt;
  Float_t Jet_withPtd;
  ULong64_t event;
  Float_t genWeight;
  Float_t isEle;
  Float_t isMu;
  Float_t isOther;
  ULong64_t luminosityBlock;
  Float_t met_phi;
  Float_t met_pt;
  Float_t nPVs;
  Float_t nPVs_good;
  Float_t rho;
  UInt_t run;

  void reset(const panda::Event& e);
  void fill();
  void write(TObject* obj) { f->WriteTObject(obj, obj->GetName()); }
  
  enum class jet {
    Jet
  };
  template <typename G> void set_jet(const jet base, const panda::Jet& jet, const gennujet::GenNuVec& withnu, const G gen);

 private:
  TFile* f;
  TTree* t;

};

regfile::regfile(const char* outfile_name, const char* name)
: f {new TFile(outfile_name, "CREATE", "", 1)},
  t {new TTree(name, name)}
{
  t->Branch("Jet_btagCMVA", &Jet_btagCMVA, "Jet_btagCMVA/F");
  t->Branch("Jet_btagDeepB", &Jet_btagDeepB, "Jet_btagDeepB/F");
  t->Branch("Jet_btagDeepC", &Jet_btagDeepC, "Jet_btagDeepC/F");
  t->Branch("Jet_chEmEF", &Jet_chEmEF, "Jet_chEmEF/F");
  t->Branch("Jet_chHEF", &Jet_chHEF, "Jet_chHEF/F");
  t->Branch("Jet_e", &Jet_e, "Jet_e/F");
  t->Branch("Jet_energyRing_dR0_ch", &Jet_energyRing_dR0_ch, "Jet_energyRing_dR0_ch/F");
  t->Branch("Jet_energyRing_dR0_ch_Jet_rawEnergy", &Jet_energyRing_dR0_ch_Jet_rawEnergy, "Jet_energyRing_dR0_ch_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR0_em", &Jet_energyRing_dR0_em, "Jet_energyRing_dR0_em/F");
  t->Branch("Jet_energyRing_dR0_em_Jet_rawEnergy", &Jet_energyRing_dR0_em_Jet_rawEnergy, "Jet_energyRing_dR0_em_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR0_mu", &Jet_energyRing_dR0_mu, "Jet_energyRing_dR0_mu/F");
  t->Branch("Jet_energyRing_dR0_mu_Jet_rawEnergy", &Jet_energyRing_dR0_mu_Jet_rawEnergy, "Jet_energyRing_dR0_mu_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR0_neut", &Jet_energyRing_dR0_neut, "Jet_energyRing_dR0_neut/F");
  t->Branch("Jet_energyRing_dR0_neut_Jet_rawEnergy", &Jet_energyRing_dR0_neut_Jet_rawEnergy, "Jet_energyRing_dR0_neut_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR1_ch", &Jet_energyRing_dR1_ch, "Jet_energyRing_dR1_ch/F");
  t->Branch("Jet_energyRing_dR1_ch_Jet_rawEnergy", &Jet_energyRing_dR1_ch_Jet_rawEnergy, "Jet_energyRing_dR1_ch_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR1_em", &Jet_energyRing_dR1_em, "Jet_energyRing_dR1_em/F");
  t->Branch("Jet_energyRing_dR1_em_Jet_rawEnergy", &Jet_energyRing_dR1_em_Jet_rawEnergy, "Jet_energyRing_dR1_em_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR1_mu", &Jet_energyRing_dR1_mu, "Jet_energyRing_dR1_mu/F");
  t->Branch("Jet_energyRing_dR1_mu_Jet_rawEnergy", &Jet_energyRing_dR1_mu_Jet_rawEnergy, "Jet_energyRing_dR1_mu_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR1_neut", &Jet_energyRing_dR1_neut, "Jet_energyRing_dR1_neut/F");
  t->Branch("Jet_energyRing_dR1_neut_Jet_rawEnergy", &Jet_energyRing_dR1_neut_Jet_rawEnergy, "Jet_energyRing_dR1_neut_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR2_ch", &Jet_energyRing_dR2_ch, "Jet_energyRing_dR2_ch/F");
  t->Branch("Jet_energyRing_dR2_ch_Jet_rawEnergy", &Jet_energyRing_dR2_ch_Jet_rawEnergy, "Jet_energyRing_dR2_ch_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR2_em", &Jet_energyRing_dR2_em, "Jet_energyRing_dR2_em/F");
  t->Branch("Jet_energyRing_dR2_em_Jet_rawEnergy", &Jet_energyRing_dR2_em_Jet_rawEnergy, "Jet_energyRing_dR2_em_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR2_mu", &Jet_energyRing_dR2_mu, "Jet_energyRing_dR2_mu/F");
  t->Branch("Jet_energyRing_dR2_mu_Jet_rawEnergy", &Jet_energyRing_dR2_mu_Jet_rawEnergy, "Jet_energyRing_dR2_mu_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR2_neut", &Jet_energyRing_dR2_neut, "Jet_energyRing_dR2_neut/F");
  t->Branch("Jet_energyRing_dR2_neut_Jet_rawEnergy", &Jet_energyRing_dR2_neut_Jet_rawEnergy, "Jet_energyRing_dR2_neut_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR3_ch", &Jet_energyRing_dR3_ch, "Jet_energyRing_dR3_ch/F");
  t->Branch("Jet_energyRing_dR3_ch_Jet_rawEnergy", &Jet_energyRing_dR3_ch_Jet_rawEnergy, "Jet_energyRing_dR3_ch_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR3_em", &Jet_energyRing_dR3_em, "Jet_energyRing_dR3_em/F");
  t->Branch("Jet_energyRing_dR3_em_Jet_rawEnergy", &Jet_energyRing_dR3_em_Jet_rawEnergy, "Jet_energyRing_dR3_em_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR3_mu", &Jet_energyRing_dR3_mu, "Jet_energyRing_dR3_mu/F");
  t->Branch("Jet_energyRing_dR3_mu_Jet_rawEnergy", &Jet_energyRing_dR3_mu_Jet_rawEnergy, "Jet_energyRing_dR3_mu_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR3_neut", &Jet_energyRing_dR3_neut, "Jet_energyRing_dR3_neut/F");
  t->Branch("Jet_energyRing_dR3_neut_Jet_rawEnergy", &Jet_energyRing_dR3_neut_Jet_rawEnergy, "Jet_energyRing_dR3_neut_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR4_ch", &Jet_energyRing_dR4_ch, "Jet_energyRing_dR4_ch/F");
  t->Branch("Jet_energyRing_dR4_ch_Jet_rawEnergy", &Jet_energyRing_dR4_ch_Jet_rawEnergy, "Jet_energyRing_dR4_ch_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR4_em", &Jet_energyRing_dR4_em, "Jet_energyRing_dR4_em/F");
  t->Branch("Jet_energyRing_dR4_em_Jet_rawEnergy", &Jet_energyRing_dR4_em_Jet_rawEnergy, "Jet_energyRing_dR4_em_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR4_mu", &Jet_energyRing_dR4_mu, "Jet_energyRing_dR4_mu/F");
  t->Branch("Jet_energyRing_dR4_mu_Jet_rawEnergy", &Jet_energyRing_dR4_mu_Jet_rawEnergy, "Jet_energyRing_dR4_mu_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR4_neut", &Jet_energyRing_dR4_neut, "Jet_energyRing_dR4_neut/F");
  t->Branch("Jet_energyRing_dR4_neut_Jet_rawEnergy", &Jet_energyRing_dR4_neut_Jet_rawEnergy, "Jet_energyRing_dR4_neut_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR5_ch", &Jet_energyRing_dR5_ch, "Jet_energyRing_dR5_ch/F");
  t->Branch("Jet_energyRing_dR5_ch_Jet_rawEnergy", &Jet_energyRing_dR5_ch_Jet_rawEnergy, "Jet_energyRing_dR5_ch_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR5_em", &Jet_energyRing_dR5_em, "Jet_energyRing_dR5_em/F");
  t->Branch("Jet_energyRing_dR5_em_Jet_rawEnergy", &Jet_energyRing_dR5_em_Jet_rawEnergy, "Jet_energyRing_dR5_em_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR5_mu", &Jet_energyRing_dR5_mu, "Jet_energyRing_dR5_mu/F");
  t->Branch("Jet_energyRing_dR5_mu_Jet_rawEnergy", &Jet_energyRing_dR5_mu_Jet_rawEnergy, "Jet_energyRing_dR5_mu_Jet_rawEnergy/F");
  t->Branch("Jet_energyRing_dR5_neut", &Jet_energyRing_dR5_neut, "Jet_energyRing_dR5_neut/F");
  t->Branch("Jet_energyRing_dR5_neut_Jet_rawEnergy", &Jet_energyRing_dR5_neut_Jet_rawEnergy, "Jet_energyRing_dR5_neut_Jet_rawEnergy/F");
  t->Branch("Jet_eta", &Jet_eta, "Jet_eta/F");
  t->Branch("Jet_leadTrackPt", &Jet_leadTrackPt, "Jet_leadTrackPt/F");
  t->Branch("Jet_leptonDeltaR", &Jet_leptonDeltaR, "Jet_leptonDeltaR/F");
  t->Branch("Jet_leptonPdgId", &Jet_leptonPdgId, "Jet_leptonPdgId/I");
  t->Branch("Jet_leptonPt", &Jet_leptonPt, "Jet_leptonPt/F");
  t->Branch("Jet_leptonPtRel", &Jet_leptonPtRel, "Jet_leptonPtRel/F");
  t->Branch("Jet_leptonPtRelInv", &Jet_leptonPtRelInv, "Jet_leptonPtRelInv/F");
  t->Branch("Jet_mass", &Jet_mass, "Jet_mass/F");
  t->Branch("Jet_mcEta", &Jet_mcEta, "Jet_mcEta/F");
  t->Branch("Jet_mcFlavour", &Jet_mcFlavour, "Jet_mcFlavour/F");
  t->Branch("Jet_mcM", &Jet_mcM, "Jet_mcM/F");
  t->Branch("Jet_mcPhi", &Jet_mcPhi, "Jet_mcPhi/F");
  t->Branch("Jet_mcPt", &Jet_mcPt, "Jet_mcPt/F");
  t->Branch("Jet_met_dPhi", &Jet_met_dPhi, "Jet_met_dPhi/F");
  t->Branch("Jet_mt", &Jet_mt, "Jet_mt/F");
  t->Branch("Jet_neEmEF", &Jet_neEmEF, "Jet_neEmEF/F");
  t->Branch("Jet_neHEF", &Jet_neHEF, "Jet_neHEF/F");
  t->Branch("Jet_numDaughters_pt03", &Jet_numDaughters_pt03, "Jet_numDaughters_pt03/s");
  t->Branch("Jet_numberOfDaughters", &Jet_numberOfDaughters, "Jet_numberOfDaughters/s");
  t->Branch("Jet_pf_0_deta", &Jet_pf_0_deta, "Jet_pf_0_deta/F");
  t->Branch("Jet_pf_0_dphi", &Jet_pf_0_dphi, "Jet_pf_0_dphi/F");
  t->Branch("Jet_pf_0_dxy", &Jet_pf_0_dxy, "Jet_pf_0_dxy/F");
  t->Branch("Jet_pf_0_dz", &Jet_pf_0_dz, "Jet_pf_0_dz/F");
  t->Branch("Jet_pf_0_pdgid", &Jet_pf_0_pdgid, "Jet_pf_0_pdgid/I");
  t->Branch("Jet_pf_0_ptfrac", &Jet_pf_0_ptfrac, "Jet_pf_0_ptfrac/F");
  t->Branch("Jet_pf_0_puppiwt", &Jet_pf_0_puppiwt, "Jet_pf_0_puppiwt/F");
  t->Branch("Jet_pf_1_deta", &Jet_pf_1_deta, "Jet_pf_1_deta/F");
  t->Branch("Jet_pf_1_dphi", &Jet_pf_1_dphi, "Jet_pf_1_dphi/F");
  t->Branch("Jet_pf_1_dxy", &Jet_pf_1_dxy, "Jet_pf_1_dxy/F");
  t->Branch("Jet_pf_1_dz", &Jet_pf_1_dz, "Jet_pf_1_dz/F");
  t->Branch("Jet_pf_1_pdgid", &Jet_pf_1_pdgid, "Jet_pf_1_pdgid/I");
  t->Branch("Jet_pf_1_ptfrac", &Jet_pf_1_ptfrac, "Jet_pf_1_ptfrac/F");
  t->Branch("Jet_pf_1_puppiwt", &Jet_pf_1_puppiwt, "Jet_pf_1_puppiwt/F");
  t->Branch("Jet_pf_2_deta", &Jet_pf_2_deta, "Jet_pf_2_deta/F");
  t->Branch("Jet_pf_2_dphi", &Jet_pf_2_dphi, "Jet_pf_2_dphi/F");
  t->Branch("Jet_pf_2_dxy", &Jet_pf_2_dxy, "Jet_pf_2_dxy/F");
  t->Branch("Jet_pf_2_dz", &Jet_pf_2_dz, "Jet_pf_2_dz/F");
  t->Branch("Jet_pf_2_pdgid", &Jet_pf_2_pdgid, "Jet_pf_2_pdgid/I");
  t->Branch("Jet_pf_2_ptfrac", &Jet_pf_2_ptfrac, "Jet_pf_2_ptfrac/F");
  t->Branch("Jet_pf_2_puppiwt", &Jet_pf_2_puppiwt, "Jet_pf_2_puppiwt/F");
  t->Branch("Jet_pf_3_deta", &Jet_pf_3_deta, "Jet_pf_3_deta/F");
  t->Branch("Jet_pf_3_dphi", &Jet_pf_3_dphi, "Jet_pf_3_dphi/F");
  t->Branch("Jet_pf_3_dxy", &Jet_pf_3_dxy, "Jet_pf_3_dxy/F");
  t->Branch("Jet_pf_3_dz", &Jet_pf_3_dz, "Jet_pf_3_dz/F");
  t->Branch("Jet_pf_3_pdgid", &Jet_pf_3_pdgid, "Jet_pf_3_pdgid/I");
  t->Branch("Jet_pf_3_ptfrac", &Jet_pf_3_ptfrac, "Jet_pf_3_ptfrac/F");
  t->Branch("Jet_pf_3_puppiwt", &Jet_pf_3_puppiwt, "Jet_pf_3_puppiwt/F");
  t->Branch("Jet_pf_4_deta", &Jet_pf_4_deta, "Jet_pf_4_deta/F");
  t->Branch("Jet_pf_4_dphi", &Jet_pf_4_dphi, "Jet_pf_4_dphi/F");
  t->Branch("Jet_pf_4_dxy", &Jet_pf_4_dxy, "Jet_pf_4_dxy/F");
  t->Branch("Jet_pf_4_dz", &Jet_pf_4_dz, "Jet_pf_4_dz/F");
  t->Branch("Jet_pf_4_pdgid", &Jet_pf_4_pdgid, "Jet_pf_4_pdgid/I");
  t->Branch("Jet_pf_4_ptfrac", &Jet_pf_4_ptfrac, "Jet_pf_4_ptfrac/F");
  t->Branch("Jet_pf_4_puppiwt", &Jet_pf_4_puppiwt, "Jet_pf_4_puppiwt/F");
  t->Branch("Jet_pf_fastjet_sorted_eta", &Jet_pf_fastjet_sorted_eta);
  t->Branch("Jet_pf_fastjet_sorted_m", &Jet_pf_fastjet_sorted_m);
  t->Branch("Jet_pf_fastjet_sorted_phi", &Jet_pf_fastjet_sorted_phi);
  t->Branch("Jet_pf_fastjet_sorted_pt", &Jet_pf_fastjet_sorted_pt);
  t->Branch("Jet_pf_pt_sorted_eta", &Jet_pf_pt_sorted_eta);
  t->Branch("Jet_pf_pt_sorted_m", &Jet_pf_pt_sorted_m);
  t->Branch("Jet_pf_pt_sorted_phi", &Jet_pf_pt_sorted_phi);
  t->Branch("Jet_pf_pt_sorted_pt", &Jet_pf_pt_sorted_pt);
  t->Branch("Jet_phi", &Jet_phi, "Jet_phi/F");
  t->Branch("Jet_pt", &Jet_pt, "Jet_pt/F");
  t->Branch("Jet_ptd", &Jet_ptd, "Jet_ptd/F");
  t->Branch("Jet_puId", &Jet_puId, "Jet_puId/F");
  t->Branch("Jet_puppi_charged_deta", &Jet_puppi_charged_deta, "Jet_puppi_charged_deta/F");
  t->Branch("Jet_puppi_charged_dphi", &Jet_puppi_charged_dphi, "Jet_puppi_charged_dphi/F");
  t->Branch("Jet_puppi_charged_e", &Jet_puppi_charged_e, "Jet_puppi_charged_e/F");
  t->Branch("Jet_puppi_charged_eta", &Jet_puppi_charged_eta, "Jet_puppi_charged_eta/F");
  t->Branch("Jet_puppi_charged_m", &Jet_puppi_charged_m, "Jet_puppi_charged_m/F");
  t->Branch("Jet_puppi_charged_phi", &Jet_puppi_charged_phi, "Jet_puppi_charged_phi/F");
  t->Branch("Jet_puppi_charged_pt", &Jet_puppi_charged_pt, "Jet_puppi_charged_pt/F");
  t->Branch("Jet_puppi_charged_ptfrac", &Jet_puppi_charged_ptfrac, "Jet_puppi_charged_ptfrac/F");
  t->Branch("Jet_puppi_charged_pu_deta", &Jet_puppi_charged_pu_deta, "Jet_puppi_charged_pu_deta/F");
  t->Branch("Jet_puppi_charged_pu_dphi", &Jet_puppi_charged_pu_dphi, "Jet_puppi_charged_pu_dphi/F");
  t->Branch("Jet_puppi_charged_pu_e", &Jet_puppi_charged_pu_e, "Jet_puppi_charged_pu_e/F");
  t->Branch("Jet_puppi_charged_pu_eta", &Jet_puppi_charged_pu_eta, "Jet_puppi_charged_pu_eta/F");
  t->Branch("Jet_puppi_charged_pu_m", &Jet_puppi_charged_pu_m, "Jet_puppi_charged_pu_m/F");
  t->Branch("Jet_puppi_charged_pu_phi", &Jet_puppi_charged_pu_phi, "Jet_puppi_charged_pu_phi/F");
  t->Branch("Jet_puppi_charged_pu_pt", &Jet_puppi_charged_pu_pt, "Jet_puppi_charged_pu_pt/F");
  t->Branch("Jet_puppi_charged_pu_ptfrac", &Jet_puppi_charged_pu_ptfrac, "Jet_puppi_charged_pu_ptfrac/F");
  t->Branch("Jet_puppi_neutral_deta", &Jet_puppi_neutral_deta, "Jet_puppi_neutral_deta/F");
  t->Branch("Jet_puppi_neutral_dphi", &Jet_puppi_neutral_dphi, "Jet_puppi_neutral_dphi/F");
  t->Branch("Jet_puppi_neutral_e", &Jet_puppi_neutral_e, "Jet_puppi_neutral_e/F");
  t->Branch("Jet_puppi_neutral_eta", &Jet_puppi_neutral_eta, "Jet_puppi_neutral_eta/F");
  t->Branch("Jet_puppi_neutral_m", &Jet_puppi_neutral_m, "Jet_puppi_neutral_m/F");
  t->Branch("Jet_puppi_neutral_phi", &Jet_puppi_neutral_phi, "Jet_puppi_neutral_phi/F");
  t->Branch("Jet_puppi_neutral_pt", &Jet_puppi_neutral_pt, "Jet_puppi_neutral_pt/F");
  t->Branch("Jet_puppi_neutral_ptfrac", &Jet_puppi_neutral_ptfrac, "Jet_puppi_neutral_ptfrac/F");
  t->Branch("Jet_puppi_neutral_pu_deta", &Jet_puppi_neutral_pu_deta, "Jet_puppi_neutral_pu_deta/F");
  t->Branch("Jet_puppi_neutral_pu_dphi", &Jet_puppi_neutral_pu_dphi, "Jet_puppi_neutral_pu_dphi/F");
  t->Branch("Jet_puppi_neutral_pu_e", &Jet_puppi_neutral_pu_e, "Jet_puppi_neutral_pu_e/F");
  t->Branch("Jet_puppi_neutral_pu_eta", &Jet_puppi_neutral_pu_eta, "Jet_puppi_neutral_pu_eta/F");
  t->Branch("Jet_puppi_neutral_pu_m", &Jet_puppi_neutral_pu_m, "Jet_puppi_neutral_pu_m/F");
  t->Branch("Jet_puppi_neutral_pu_phi", &Jet_puppi_neutral_pu_phi, "Jet_puppi_neutral_pu_phi/F");
  t->Branch("Jet_puppi_neutral_pu_pt", &Jet_puppi_neutral_pu_pt, "Jet_puppi_neutral_pu_pt/F");
  t->Branch("Jet_puppi_neutral_pu_ptfrac", &Jet_puppi_neutral_pu_ptfrac, "Jet_puppi_neutral_pu_ptfrac/F");
  t->Branch("Jet_raw", &Jet_raw, "Jet_raw/F");
  t->Branch("Jet_rawEnergy", &Jet_rawEnergy, "Jet_rawEnergy/F");
  t->Branch("Jet_rawFactor", &Jet_rawFactor, "Jet_rawFactor/F");
  t->Branch("Jet_tf_190711_0_deta", &Jet_tf_190711_0_deta, "Jet_tf_190711_0_deta/F");
  t->Branch("Jet_tf_190711_0_dphi", &Jet_tf_190711_0_dphi, "Jet_tf_190711_0_dphi/F");
  t->Branch("Jet_tf_190711_0_mratio", &Jet_tf_190711_0_mratio, "Jet_tf_190711_0_mratio/F");
  t->Branch("Jet_tf_190711_0_ptratio", &Jet_tf_190711_0_ptratio, "Jet_tf_190711_0_ptratio/F");
  t->Branch("Jet_tf_190711_2_deta", &Jet_tf_190711_2_deta, "Jet_tf_190711_2_deta/F");
  t->Branch("Jet_tf_190711_2_dphi", &Jet_tf_190711_2_dphi, "Jet_tf_190711_2_dphi/F");
  t->Branch("Jet_vtx3dL", &Jet_vtx3dL, "Jet_vtx3dL/F");
  t->Branch("Jet_vtx3deL", &Jet_vtx3deL, "Jet_vtx3deL/F");
  t->Branch("Jet_vtxMass", &Jet_vtxMass, "Jet_vtxMass/F");
  t->Branch("Jet_vtxNtrk", &Jet_vtxNtrk, "Jet_vtxNtrk/S");
  t->Branch("Jet_vtxPt", &Jet_vtxPt, "Jet_vtxPt/F");
  t->Branch("Jet_withPtd", &Jet_withPtd, "Jet_withPtd/F");
  t->Branch("event", &event, "event/l");
  t->Branch("genWeight", &genWeight, "genWeight/F");
  t->Branch("isEle", &isEle, "isEle/F");
  t->Branch("isMu", &isMu, "isMu/F");
  t->Branch("isOther", &isOther, "isOther/F");
  t->Branch("luminosityBlock", &luminosityBlock, "luminosityBlock/l");
  t->Branch("met_phi", &met_phi, "met_phi/F");
  t->Branch("met_pt", &met_pt, "met_pt/F");
  t->Branch("nPVs", &nPVs, "nPVs/F");
  t->Branch("nPVs_good", &nPVs_good, "nPVs_good/F");
  t->Branch("rho", &rho, "rho/F");
  t->Branch("run", &run, "run/i");
}

void regfile::reset(const panda::Event& e) {
  Jet_btagCMVA = 0;
  Jet_btagDeepB = 0;
  Jet_btagDeepC = 0;
  Jet_chEmEF = 0;
  Jet_chHEF = 0;
  Jet_e = 0;
  Jet_energyRing_dR0_ch = 0;
  Jet_energyRing_dR0_ch_Jet_rawEnergy = 0;
  Jet_energyRing_dR0_em = 0;
  Jet_energyRing_dR0_em_Jet_rawEnergy = 0;
  Jet_energyRing_dR0_mu = 0;
  Jet_energyRing_dR0_mu_Jet_rawEnergy = 0;
  Jet_energyRing_dR0_neut = 0;
  Jet_energyRing_dR0_neut_Jet_rawEnergy = 0;
  Jet_energyRing_dR1_ch = 0;
  Jet_energyRing_dR1_ch_Jet_rawEnergy = 0;
  Jet_energyRing_dR1_em = 0;
  Jet_energyRing_dR1_em_Jet_rawEnergy = 0;
  Jet_energyRing_dR1_mu = 0;
  Jet_energyRing_dR1_mu_Jet_rawEnergy = 0;
  Jet_energyRing_dR1_neut = 0;
  Jet_energyRing_dR1_neut_Jet_rawEnergy = 0;
  Jet_energyRing_dR2_ch = 0;
  Jet_energyRing_dR2_ch_Jet_rawEnergy = 0;
  Jet_energyRing_dR2_em = 0;
  Jet_energyRing_dR2_em_Jet_rawEnergy = 0;
  Jet_energyRing_dR2_mu = 0;
  Jet_energyRing_dR2_mu_Jet_rawEnergy = 0;
  Jet_energyRing_dR2_neut = 0;
  Jet_energyRing_dR2_neut_Jet_rawEnergy = 0;
  Jet_energyRing_dR3_ch = 0;
  Jet_energyRing_dR3_ch_Jet_rawEnergy = 0;
  Jet_energyRing_dR3_em = 0;
  Jet_energyRing_dR3_em_Jet_rawEnergy = 0;
  Jet_energyRing_dR3_mu = 0;
  Jet_energyRing_dR3_mu_Jet_rawEnergy = 0;
  Jet_energyRing_dR3_neut = 0;
  Jet_energyRing_dR3_neut_Jet_rawEnergy = 0;
  Jet_energyRing_dR4_ch = 0;
  Jet_energyRing_dR4_ch_Jet_rawEnergy = 0;
  Jet_energyRing_dR4_em = 0;
  Jet_energyRing_dR4_em_Jet_rawEnergy = 0;
  Jet_energyRing_dR4_mu = 0;
  Jet_energyRing_dR4_mu_Jet_rawEnergy = 0;
  Jet_energyRing_dR4_neut = 0;
  Jet_energyRing_dR4_neut_Jet_rawEnergy = 0;
  Jet_energyRing_dR5_ch = 0;
  Jet_energyRing_dR5_ch_Jet_rawEnergy = 0;
  Jet_energyRing_dR5_em = 0;
  Jet_energyRing_dR5_em_Jet_rawEnergy = 0;
  Jet_energyRing_dR5_mu = 0;
  Jet_energyRing_dR5_mu_Jet_rawEnergy = 0;
  Jet_energyRing_dR5_neut = 0;
  Jet_energyRing_dR5_neut_Jet_rawEnergy = 0;
  Jet_eta = 0;
  Jet_leadTrackPt = 0;
  Jet_leptonDeltaR = 0;
  Jet_leptonPdgId = 0;
  Jet_leptonPt = 0;
  Jet_leptonPtRel = 0;
  Jet_leptonPtRelInv = 0;
  Jet_mass = 0;
  Jet_mcEta = 0;
  Jet_mcFlavour = 0;
  Jet_mcM = 0;
  Jet_mcPhi = 0;
  Jet_mcPt = 0;
  Jet_met_dPhi = 0;
  Jet_mt = 0;
  Jet_neEmEF = 0;
  Jet_neHEF = 0;
  Jet_numDaughters_pt03 = 0;
  Jet_numberOfDaughters = 0;
  Jet_pf_0_deta = 0;
  Jet_pf_0_dphi = 0;
  Jet_pf_0_dxy = 0;
  Jet_pf_0_dz = 0;
  Jet_pf_0_pdgid = 0;
  Jet_pf_0_ptfrac = 0;
  Jet_pf_0_puppiwt = 0;
  Jet_pf_1_deta = 0;
  Jet_pf_1_dphi = 0;
  Jet_pf_1_dxy = 0;
  Jet_pf_1_dz = 0;
  Jet_pf_1_pdgid = 0;
  Jet_pf_1_ptfrac = 0;
  Jet_pf_1_puppiwt = 0;
  Jet_pf_2_deta = 0;
  Jet_pf_2_dphi = 0;
  Jet_pf_2_dxy = 0;
  Jet_pf_2_dz = 0;
  Jet_pf_2_pdgid = 0;
  Jet_pf_2_ptfrac = 0;
  Jet_pf_2_puppiwt = 0;
  Jet_pf_3_deta = 0;
  Jet_pf_3_dphi = 0;
  Jet_pf_3_dxy = 0;
  Jet_pf_3_dz = 0;
  Jet_pf_3_pdgid = 0;
  Jet_pf_3_ptfrac = 0;
  Jet_pf_3_puppiwt = 0;
  Jet_pf_4_deta = 0;
  Jet_pf_4_dphi = 0;
  Jet_pf_4_dxy = 0;
  Jet_pf_4_dz = 0;
  Jet_pf_4_pdgid = 0;
  Jet_pf_4_ptfrac = 0;
  Jet_pf_4_puppiwt = 0;
  Jet_pf_fastjet_sorted_eta.clear();
  Jet_pf_fastjet_sorted_m.clear();
  Jet_pf_fastjet_sorted_phi.clear();
  Jet_pf_fastjet_sorted_pt.clear();
  Jet_pf_pt_sorted_eta.clear();
  Jet_pf_pt_sorted_m.clear();
  Jet_pf_pt_sorted_phi.clear();
  Jet_pf_pt_sorted_pt.clear();
  Jet_phi = 0;
  Jet_pt = 0;
  Jet_ptd = 0;
  Jet_puId = 0;
  Jet_puppi_charged_deta = 0;
  Jet_puppi_charged_dphi = 0;
  Jet_puppi_charged_e = 0;
  Jet_puppi_charged_eta = 0;
  Jet_puppi_charged_m = 0;
  Jet_puppi_charged_phi = 0;
  Jet_puppi_charged_pt = 0;
  Jet_puppi_charged_ptfrac = 0;
  Jet_puppi_charged_pu_deta = 0;
  Jet_puppi_charged_pu_dphi = 0;
  Jet_puppi_charged_pu_e = 0;
  Jet_puppi_charged_pu_eta = 0;
  Jet_puppi_charged_pu_m = 0;
  Jet_puppi_charged_pu_phi = 0;
  Jet_puppi_charged_pu_pt = 0;
  Jet_puppi_charged_pu_ptfrac = 0;
  Jet_puppi_neutral_deta = 0;
  Jet_puppi_neutral_dphi = 0;
  Jet_puppi_neutral_e = 0;
  Jet_puppi_neutral_eta = 0;
  Jet_puppi_neutral_m = 0;
  Jet_puppi_neutral_phi = 0;
  Jet_puppi_neutral_pt = 0;
  Jet_puppi_neutral_ptfrac = 0;
  Jet_puppi_neutral_pu_deta = 0;
  Jet_puppi_neutral_pu_dphi = 0;
  Jet_puppi_neutral_pu_e = 0;
  Jet_puppi_neutral_pu_eta = 0;
  Jet_puppi_neutral_pu_m = 0;
  Jet_puppi_neutral_pu_phi = 0;
  Jet_puppi_neutral_pu_pt = 0;
  Jet_puppi_neutral_pu_ptfrac = 0;
  Jet_raw = 0;
  Jet_rawEnergy = 0;
  Jet_rawFactor = 0;
  Jet_tf_190711_0.clear();
  Jet_tf_190711_0_deta = 0;
  Jet_tf_190711_0_dphi = 0;
  Jet_tf_190711_0_mratio = 0;
  Jet_tf_190711_0_ptratio = 0;
  Jet_tf_190711_2.clear();
  Jet_tf_190711_2_deta = 0;
  Jet_tf_190711_2_dphi = 0;
  Jet_vtx3dL = 0;
  Jet_vtx3deL = 0;
  Jet_vtxMass = 0;
  Jet_vtxNtrk = 0;
  Jet_vtxPt = 0;
  Jet_withPtd = 0;
  event = e.eventNumber;
  genWeight = e.weight;
  isEle = 0;
  isMu = 0;
  isOther = 0;
  luminosityBlock = e.lumiNumber;
  met_phi = e.pfMet.phi;
  met_pt = e.pfMet.pt;
  nPVs = e.npvTrue;
  nPVs_good = e.npv;
  rho = e.rho;
  run = e.runNumber;
}

void regfile::fill() {
  isEle = std::abs(Jet_leptonPdgId) == 11;
  isMu = std::abs(Jet_leptonPdgId) == 13;
  isOther = not (isEle or isMu);
  Jet_withPtd = Jet_ptd;
  Jet_met_dPhi = TVector2::Phi_mpi_pi(met_phi - Jet_phi);
  Jet_puppi_charged_ptfrac = Jet_puppi_charged_pt/Jet_pt;
  Jet_puppi_charged_pu_ptfrac = Jet_puppi_charged_pu_pt/Jet_pt;
  Jet_puppi_neutral_ptfrac = Jet_puppi_neutral_pt/Jet_pt;
  Jet_puppi_neutral_pu_ptfrac = Jet_puppi_neutral_pu_pt/Jet_pt;
  Jet_puppi_charged_deta = TVector2::Phi_mpi_pi(Jet_puppi_charged_eta - Jet_eta);
  Jet_puppi_charged_dphi = TVector2::Phi_mpi_pi(Jet_puppi_charged_phi - Jet_phi);
  Jet_puppi_charged_pu_deta = TVector2::Phi_mpi_pi(Jet_puppi_charged_pu_eta - Jet_eta);
  Jet_puppi_charged_pu_dphi = TVector2::Phi_mpi_pi(Jet_puppi_charged_pu_phi - Jet_phi);
  Jet_puppi_neutral_deta = TVector2::Phi_mpi_pi(Jet_puppi_neutral_eta - Jet_eta);
  Jet_puppi_neutral_dphi = TVector2::Phi_mpi_pi(Jet_puppi_neutral_phi - Jet_phi);
  Jet_puppi_neutral_pu_deta = TVector2::Phi_mpi_pi(Jet_puppi_neutral_pu_eta - Jet_eta);
  Jet_puppi_neutral_pu_dphi = TVector2::Phi_mpi_pi(Jet_puppi_neutral_pu_phi - Jet_phi);
  Jet_tf_190711_0 = lazytf::eval("data/freeze_190711_0.pb", "data/regression9.txt", {"dnn/split_logits/Slice", "dnn/split_logits/Slice_1", "dnn/split_logits/Slice_2", "dnn/split_logits/Slice_3"}, 1, Jet_chEmEF, Jet_chHEF, Jet_energyRing_dR0_ch_Jet_rawEnergy, Jet_energyRing_dR0_em_Jet_rawEnergy, Jet_energyRing_dR0_mu_Jet_rawEnergy, Jet_energyRing_dR0_neut_Jet_rawEnergy, Jet_energyRing_dR1_ch_Jet_rawEnergy, Jet_energyRing_dR1_em_Jet_rawEnergy, Jet_energyRing_dR1_mu_Jet_rawEnergy, Jet_energyRing_dR1_neut_Jet_rawEnergy, Jet_energyRing_dR2_ch_Jet_rawEnergy, Jet_energyRing_dR2_em_Jet_rawEnergy, Jet_energyRing_dR2_mu_Jet_rawEnergy, Jet_energyRing_dR2_neut_Jet_rawEnergy, Jet_energyRing_dR3_ch_Jet_rawEnergy, Jet_energyRing_dR3_em_Jet_rawEnergy, Jet_energyRing_dR3_mu_Jet_rawEnergy, Jet_energyRing_dR3_neut_Jet_rawEnergy, Jet_energyRing_dR4_ch_Jet_rawEnergy, Jet_energyRing_dR4_em_Jet_rawEnergy, Jet_energyRing_dR4_mu_Jet_rawEnergy, Jet_energyRing_dR4_neut_Jet_rawEnergy, Jet_eta, Jet_leadTrackPt, Jet_leptonDeltaR, Jet_leptonPtRel, Jet_leptonPtRelInv, Jet_mass, Jet_mt, Jet_neEmEF, Jet_neHEF, Jet_numDaughters_pt03, Jet_pt, Jet_ptd, Jet_vtx3dL, Jet_vtx3deL, Jet_vtxMass, Jet_vtxNtrk, Jet_vtxPt, isEle, isMu, isOther, rho, Jet_pf_0_deta, Jet_pf_0_dphi, Jet_pf_0_dxy, Jet_pf_0_dz, Jet_pf_0_pdgid, Jet_pf_0_ptfrac, Jet_pf_0_puppiwt, Jet_pf_1_deta, Jet_pf_1_dphi, Jet_pf_1_dxy, Jet_pf_1_dz, Jet_pf_1_pdgid, Jet_pf_1_ptfrac, Jet_pf_1_puppiwt, Jet_pf_2_deta, Jet_pf_2_dphi, Jet_pf_2_dxy, Jet_pf_2_dz, Jet_pf_2_pdgid, Jet_pf_2_ptfrac, Jet_pf_2_puppiwt, Jet_pf_3_deta, Jet_pf_3_dphi, Jet_pf_3_dxy, Jet_pf_3_dz, Jet_pf_3_pdgid, Jet_pf_3_ptfrac, Jet_pf_3_puppiwt, Jet_pf_4_deta, Jet_pf_4_dphi, Jet_pf_4_dxy, Jet_pf_4_dz, Jet_pf_4_pdgid, Jet_pf_4_ptfrac, Jet_pf_4_puppiwt, Jet_puppi_charged_deta, Jet_puppi_charged_dphi, Jet_puppi_charged_ptfrac, Jet_puppi_charged_pu_deta, Jet_puppi_charged_pu_dphi, Jet_puppi_charged_pu_ptfrac, Jet_puppi_neutral_deta, Jet_puppi_neutral_dphi, Jet_puppi_neutral_ptfrac, Jet_puppi_neutral_pu_deta, Jet_puppi_neutral_pu_dphi, Jet_puppi_neutral_pu_ptfrac);
  Jet_tf_190711_0_ptratio = Jet_tf_190711_0[0];
  Jet_tf_190711_0_deta = Jet_tf_190711_0[1];
  Jet_tf_190711_0_dphi = Jet_tf_190711_0[2];
  Jet_tf_190711_0_mratio = Jet_tf_190711_0[3];
  Jet_tf_190711_2 = lazytf::eval("data/freeze_190711_2.pb", "data/regression9.txt", {"dnn/split_logits/Slice", "dnn/split_logits/Slice_1"}, 1, Jet_chEmEF, Jet_chHEF, Jet_energyRing_dR0_ch_Jet_rawEnergy, Jet_energyRing_dR0_em_Jet_rawEnergy, Jet_energyRing_dR0_mu_Jet_rawEnergy, Jet_energyRing_dR0_neut_Jet_rawEnergy, Jet_energyRing_dR1_ch_Jet_rawEnergy, Jet_energyRing_dR1_em_Jet_rawEnergy, Jet_energyRing_dR1_mu_Jet_rawEnergy, Jet_energyRing_dR1_neut_Jet_rawEnergy, Jet_energyRing_dR2_ch_Jet_rawEnergy, Jet_energyRing_dR2_em_Jet_rawEnergy, Jet_energyRing_dR2_mu_Jet_rawEnergy, Jet_energyRing_dR2_neut_Jet_rawEnergy, Jet_energyRing_dR3_ch_Jet_rawEnergy, Jet_energyRing_dR3_em_Jet_rawEnergy, Jet_energyRing_dR3_mu_Jet_rawEnergy, Jet_energyRing_dR3_neut_Jet_rawEnergy, Jet_energyRing_dR4_ch_Jet_rawEnergy, Jet_energyRing_dR4_em_Jet_rawEnergy, Jet_energyRing_dR4_mu_Jet_rawEnergy, Jet_energyRing_dR4_neut_Jet_rawEnergy, Jet_eta, Jet_leadTrackPt, Jet_leptonDeltaR, Jet_leptonPtRel, Jet_leptonPtRelInv, Jet_mass, Jet_mt, Jet_neEmEF, Jet_neHEF, Jet_numDaughters_pt03, Jet_pt, Jet_ptd, Jet_vtx3dL, Jet_vtx3deL, Jet_vtxMass, Jet_vtxNtrk, Jet_vtxPt, isEle, isMu, isOther, rho, Jet_pf_0_deta, Jet_pf_0_dphi, Jet_pf_0_dxy, Jet_pf_0_dz, Jet_pf_0_pdgid, Jet_pf_0_ptfrac, Jet_pf_0_puppiwt, Jet_pf_1_deta, Jet_pf_1_dphi, Jet_pf_1_dxy, Jet_pf_1_dz, Jet_pf_1_pdgid, Jet_pf_1_ptfrac, Jet_pf_1_puppiwt, Jet_pf_2_deta, Jet_pf_2_dphi, Jet_pf_2_dxy, Jet_pf_2_dz, Jet_pf_2_pdgid, Jet_pf_2_ptfrac, Jet_pf_2_puppiwt, Jet_pf_3_deta, Jet_pf_3_dphi, Jet_pf_3_dxy, Jet_pf_3_dz, Jet_pf_3_pdgid, Jet_pf_3_ptfrac, Jet_pf_3_puppiwt, Jet_pf_4_deta, Jet_pf_4_dphi, Jet_pf_4_dxy, Jet_pf_4_dz, Jet_pf_4_pdgid, Jet_pf_4_ptfrac, Jet_pf_4_puppiwt, Jet_puppi_charged_deta, Jet_puppi_charged_dphi, Jet_puppi_charged_ptfrac, Jet_puppi_charged_pu_deta, Jet_puppi_charged_pu_dphi, Jet_puppi_charged_pu_ptfrac, Jet_puppi_neutral_deta, Jet_puppi_neutral_dphi, Jet_puppi_neutral_ptfrac, Jet_puppi_neutral_pu_deta, Jet_puppi_neutral_pu_dphi, Jet_puppi_neutral_pu_ptfrac);
  Jet_tf_190711_2_deta = Jet_tf_190711_2[0];
  Jet_tf_190711_2_dphi = Jet_tf_190711_2[1];
  t->Fill();
}

template <typename G> void regfile::set_jet(const jet base, const panda::Jet& jet, const gennujet::GenNuVec& withnu, const G gen) {
  auto p4 = jet.p4();
  auto info = regression::GetJetInfo(jet);
  auto pfvecs = pfvecs::get_vecs(jet);
  switch(base) {
  case regfile::jet::Jet:
    Jet_puId = jet.puid;
    Jet_btagCMVA = jet.cmva;
    Jet_btagDeepC = jet.deepCSVc + jet.deepCSVcc;
    Jet_btagDeepB = jet.deepCSVb + jet.deepCSVbb;
    Jet_raw = jet.rawPt/jet.pt();
    Jet_pt = p4.Pt();
    Jet_eta = p4.Eta();
    Jet_phi = p4.Phi();
    Jet_mt = p4.Mt();
    Jet_e = p4.E();
    Jet_mass = p4.M();
    Jet_chHEF = jet.chf;
    Jet_neHEF = jet.nhf;
    Jet_chEmEF = jet.cef;
    Jet_neEmEF = jet.nef;
    Jet_vtxNtrk = info.vtxNtrk;
    Jet_vtxMass = info.vtxMass;
    Jet_vtx3dL = info.vtx3dL;
    Jet_vtx3deL = info.vtx3deL;
    Jet_vtxPt = info.vtxPt;
    Jet_leadTrackPt = info.maxpfpt;
    Jet_rawFactor = 1 - jet.rawPt/Jet_pt;
    Jet_rawEnergy = Jet_e * jet.rawPt/Jet_pt;
    Jet_mcPt = withnu.genvec.Pt();
    Jet_mcEta = withnu.genvec.Eta();
    Jet_mcPhi = withnu.genvec.Phi();
    Jet_mcM = withnu.genvec.M();
    Jet_mcFlavour = gen->partonFlavor;
    Jet_leptonPdgId = info.leptonPdgId;
    Jet_leptonPt = info.leptonPt;
    Jet_leptonPtRel = info.leptonPtRel;
    Jet_leptonPtRelInv = info.leptonPtRelInv;
    Jet_leptonDeltaR = info.leptonDeltaR;
    Jet_ptd = info.ptD;
    Jet_energyRing_dR0_neut = info.neut_rings[0].first;
    Jet_energyRing_dR1_neut = info.neut_rings[1].first;
    Jet_energyRing_dR2_neut = info.neut_rings[2].first;
    Jet_energyRing_dR3_neut = info.neut_rings[3].first;
    Jet_energyRing_dR4_neut = info.neut_rings[4].first;
    Jet_energyRing_dR5_neut = info.neut_rings[5].first;
    Jet_energyRing_dR0_ch = info.ch_rings[0].first;
    Jet_energyRing_dR1_ch = info.ch_rings[1].first;
    Jet_energyRing_dR2_ch = info.ch_rings[2].first;
    Jet_energyRing_dR3_ch = info.ch_rings[3].first;
    Jet_energyRing_dR4_ch = info.ch_rings[4].first;
    Jet_energyRing_dR5_ch = info.ch_rings[5].first;
    Jet_energyRing_dR0_em = info.em_rings[0].first;
    Jet_energyRing_dR1_em = info.em_rings[1].first;
    Jet_energyRing_dR2_em = info.em_rings[2].first;
    Jet_energyRing_dR3_em = info.em_rings[3].first;
    Jet_energyRing_dR4_em = info.em_rings[4].first;
    Jet_energyRing_dR5_em = info.em_rings[5].first;
    Jet_energyRing_dR0_mu = info.mu_rings[0].first;
    Jet_energyRing_dR1_mu = info.mu_rings[1].first;
    Jet_energyRing_dR2_mu = info.mu_rings[2].first;
    Jet_energyRing_dR3_mu = info.mu_rings[3].first;
    Jet_energyRing_dR4_mu = info.mu_rings[4].first;
    Jet_energyRing_dR5_mu = info.mu_rings[5].first;
    Jet_energyRing_dR0_neut_Jet_rawEnergy = Jet_energyRing_dR0_neut/Jet_rawEnergy;
    Jet_energyRing_dR1_neut_Jet_rawEnergy = Jet_energyRing_dR1_neut/Jet_rawEnergy;
    Jet_energyRing_dR2_neut_Jet_rawEnergy = Jet_energyRing_dR2_neut/Jet_rawEnergy;
    Jet_energyRing_dR3_neut_Jet_rawEnergy = Jet_energyRing_dR3_neut/Jet_rawEnergy;
    Jet_energyRing_dR4_neut_Jet_rawEnergy = Jet_energyRing_dR4_neut/Jet_rawEnergy;
    Jet_energyRing_dR5_neut_Jet_rawEnergy = Jet_energyRing_dR5_neut/Jet_rawEnergy;
    Jet_energyRing_dR0_ch_Jet_rawEnergy = Jet_energyRing_dR0_ch/Jet_rawEnergy;
    Jet_energyRing_dR1_ch_Jet_rawEnergy = Jet_energyRing_dR1_ch/Jet_rawEnergy;
    Jet_energyRing_dR2_ch_Jet_rawEnergy = Jet_energyRing_dR2_ch/Jet_rawEnergy;
    Jet_energyRing_dR3_ch_Jet_rawEnergy = Jet_energyRing_dR3_ch/Jet_rawEnergy;
    Jet_energyRing_dR4_ch_Jet_rawEnergy = Jet_energyRing_dR4_ch/Jet_rawEnergy;
    Jet_energyRing_dR5_ch_Jet_rawEnergy = Jet_energyRing_dR5_ch/Jet_rawEnergy;
    Jet_energyRing_dR0_em_Jet_rawEnergy = Jet_energyRing_dR0_em/Jet_rawEnergy;
    Jet_energyRing_dR1_em_Jet_rawEnergy = Jet_energyRing_dR1_em/Jet_rawEnergy;
    Jet_energyRing_dR2_em_Jet_rawEnergy = Jet_energyRing_dR2_em/Jet_rawEnergy;
    Jet_energyRing_dR3_em_Jet_rawEnergy = Jet_energyRing_dR3_em/Jet_rawEnergy;
    Jet_energyRing_dR4_em_Jet_rawEnergy = Jet_energyRing_dR4_em/Jet_rawEnergy;
    Jet_energyRing_dR5_em_Jet_rawEnergy = Jet_energyRing_dR5_em/Jet_rawEnergy;
    Jet_energyRing_dR0_mu_Jet_rawEnergy = Jet_energyRing_dR0_mu/Jet_rawEnergy;
    Jet_energyRing_dR1_mu_Jet_rawEnergy = Jet_energyRing_dR1_mu/Jet_rawEnergy;
    Jet_energyRing_dR2_mu_Jet_rawEnergy = Jet_energyRing_dR2_mu/Jet_rawEnergy;
    Jet_energyRing_dR3_mu_Jet_rawEnergy = Jet_energyRing_dR3_mu/Jet_rawEnergy;
    Jet_energyRing_dR4_mu_Jet_rawEnergy = Jet_energyRing_dR4_mu/Jet_rawEnergy;
    Jet_energyRing_dR5_mu_Jet_rawEnergy = Jet_energyRing_dR5_mu/Jet_rawEnergy;
    Jet_numDaughters_pt03 = info.num03;
    Jet_numberOfDaughters = jet.constituents.size();
    Jet_pf_0_ptfrac = info.pfinfo[0].ptfrac;
    Jet_pf_1_ptfrac = info.pfinfo[1].ptfrac;
    Jet_pf_2_ptfrac = info.pfinfo[2].ptfrac;
    Jet_pf_3_ptfrac = info.pfinfo[3].ptfrac;
    Jet_pf_4_ptfrac = info.pfinfo[4].ptfrac;
    Jet_pf_0_deta = info.pfinfo[0].deta;
    Jet_pf_1_deta = info.pfinfo[1].deta;
    Jet_pf_2_deta = info.pfinfo[2].deta;
    Jet_pf_3_deta = info.pfinfo[3].deta;
    Jet_pf_4_deta = info.pfinfo[4].deta;
    Jet_pf_0_dphi = info.pfinfo[0].dphi;
    Jet_pf_1_dphi = info.pfinfo[1].dphi;
    Jet_pf_2_dphi = info.pfinfo[2].dphi;
    Jet_pf_3_dphi = info.pfinfo[3].dphi;
    Jet_pf_4_dphi = info.pfinfo[4].dphi;
    Jet_pf_0_puppiwt = info.pfinfo[0].puppiwt;
    Jet_pf_1_puppiwt = info.pfinfo[1].puppiwt;
    Jet_pf_2_puppiwt = info.pfinfo[2].puppiwt;
    Jet_pf_3_puppiwt = info.pfinfo[3].puppiwt;
    Jet_pf_4_puppiwt = info.pfinfo[4].puppiwt;
    Jet_pf_0_dxy = info.pfinfo[0].dxy;
    Jet_pf_1_dxy = info.pfinfo[1].dxy;
    Jet_pf_2_dxy = info.pfinfo[2].dxy;
    Jet_pf_3_dxy = info.pfinfo[3].dxy;
    Jet_pf_4_dxy = info.pfinfo[4].dxy;
    Jet_pf_0_dz = info.pfinfo[0].dz;
    Jet_pf_1_dz = info.pfinfo[1].dz;
    Jet_pf_2_dz = info.pfinfo[2].dz;
    Jet_pf_3_dz = info.pfinfo[3].dz;
    Jet_pf_4_dz = info.pfinfo[4].dz;
    Jet_pf_0_pdgid = info.pfinfo[0].pdgid;
    Jet_pf_1_pdgid = info.pfinfo[1].pdgid;
    Jet_pf_2_pdgid = info.pfinfo[2].pdgid;
    Jet_pf_3_pdgid = info.pfinfo[3].pdgid;
    Jet_pf_4_pdgid = info.pfinfo[4].pdgid;
    Jet_puppi_charged_pt = info.charged.Pt();
    Jet_puppi_charged_m = info.charged.M();
    Jet_puppi_charged_e = info.charged.E();
    Jet_puppi_charged_pu_pt = info.charged_pu.Pt();
    Jet_puppi_charged_pu_m = info.charged_pu.M();
    Jet_puppi_charged_pu_e = info.charged_pu.E();
    Jet_puppi_neutral_pt = info.neutral.Pt();
    Jet_puppi_neutral_m = info.neutral.M();
    Jet_puppi_neutral_e = info.neutral.E();
    Jet_puppi_neutral_pu_pt = info.neutral_pu.Pt();
    Jet_puppi_neutral_pu_m = info.neutral_pu.M();
    Jet_puppi_neutral_pu_e = info.neutral_pu.E();
    Jet_puppi_charged_eta = info.charged.filled ? info.charged.Eta() : Jet_eta;
    Jet_puppi_charged_phi = info.charged.filled ? info.charged.Phi() : Jet_phi;
    Jet_puppi_charged_pu_eta = info.charged_pu.filled ? info.charged_pu.Eta() : Jet_eta;
    Jet_puppi_charged_pu_phi = info.charged_pu.filled ? info.charged_pu.Phi() : Jet_phi;
    Jet_puppi_neutral_eta = info.neutral.filled ? info.neutral.Eta() : Jet_eta;
    Jet_puppi_neutral_phi = info.neutral.filled ? info.neutral.Phi() : Jet_phi;
    Jet_puppi_neutral_pu_eta = info.neutral_pu.filled ? info.neutral_pu.Eta() : Jet_eta;
    Jet_puppi_neutral_pu_phi = info.neutral_pu.filled ? info.neutral_pu.Phi() : Jet_phi;
    Jet_pf_pt_sorted_pt = std::move(pfvecs.pf_pt_sorted_pt);
    Jet_pf_pt_sorted_eta = std::move(pfvecs.pf_pt_sorted_eta);
    Jet_pf_pt_sorted_phi = std::move(pfvecs.pf_pt_sorted_phi);
    Jet_pf_pt_sorted_m = std::move(pfvecs.pf_pt_sorted_m);
    Jet_pf_fastjet_sorted_pt = std::move(pfvecs.pf_fastjet_sorted_pt);
    Jet_pf_fastjet_sorted_eta = std::move(pfvecs.pf_fastjet_sorted_eta);
    Jet_pf_fastjet_sorted_phi = std::move(pfvecs.pf_fastjet_sorted_phi);
    Jet_pf_fastjet_sorted_m = std::move(pfvecs.pf_fastjet_sorted_m);
    break;
  default:
    throw;
  }
}

#endif
