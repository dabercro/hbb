#ifndef CROMBIE_HBBFILE_H
#define CROMBIE_HBBFILE_H

#include <string>
#include <vector>
#include <unordered_map>
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "PlotTools/interface/KinematicFunctions.h"
#include "PandaTree/Objects/interface/Event.h"
#include "btagreaders.h"
#include "bcal/BTagCalibrationStandalone.h"
#include "TLorentzVector.h"
#include "TVector2.h"

class hbbfile {

 public:
  hbbfile(const char* outfile_name, const char* name = "events");
  ~hbbfile() { write(t); f->Close(); }

  Float_t calomet;
  Float_t calometphi;
  Bool_t cmva_hbb;
  Float_t cmva_hbb_eta;
  Float_t cmva_hbb_m;
  Float_t cmva_hbb_phi;
  Float_t cmva_hbb_pt;
  Bool_t cmva_jet1;
  Float_t cmva_jet1_chf;
  Float_t cmva_jet1_cmva;
  Float_t cmva_jet1_csv;
  Float_t cmva_jet1_deepcmvab;
  Float_t cmva_jet1_deepcsvb;
  Float_t cmva_jet1_emfrac;
  Float_t cmva_jet1_eta;
  Bool_t cmva_jet1_gen;
  Float_t cmva_jet1_gen_eta;
  Float_t cmva_jet1_gen_m;
  Int_t cmva_jet1_gen_num_b;
  Int_t cmva_jet1_gen_parton_flav;
  Int_t cmva_jet1_gen_pdgid;
  Float_t cmva_jet1_gen_phi;
  Float_t cmva_jet1_gen_pt;
  Float_t cmva_jet1_leadlep_dr;
  Float_t cmva_jet1_leadlep_pt;
  Float_t cmva_jet1_leadlep_ptrel;
  Float_t cmva_jet1_loose_sf;
  Float_t cmva_jet1_loose_sf_down;
  Float_t cmva_jet1_loose_sf_up;
  Float_t cmva_jet1_m;
  Float_t cmva_jet1_maxtrk;
  Float_t cmva_jet1_medium_sf;
  Float_t cmva_jet1_medium_sf_down;
  Float_t cmva_jet1_medium_sf_up;
  Float_t cmva_jet1_nhf;
  Int_t cmva_jet1_nlep;
  Float_t cmva_jet1_phi;
  Float_t cmva_jet1_pt;
  Float_t cmva_jet1_pt_ratio;
  Float_t cmva_jet1_pt_reg;
  Float_t cmva_jet1_qgl;
  Float_t cmva_jet1_tight_sf;
  Float_t cmva_jet1_tight_sf_down;
  Float_t cmva_jet1_tight_sf_up;
  Float_t cmva_jet1_vtx_3Derr;
  Float_t cmva_jet1_vtx_3Dval;
  Float_t cmva_jet1_vtx_m;
  Int_t cmva_jet1_vtx_ntrk;
  Float_t cmva_jet1_vtx_pt;
  Bool_t cmva_jet2;
  Float_t cmva_jet2_chf;
  Float_t cmva_jet2_cmva;
  Float_t cmva_jet2_csv;
  Float_t cmva_jet2_deepcmvab;
  Float_t cmva_jet2_deepcsvb;
  Float_t cmva_jet2_emfrac;
  Float_t cmva_jet2_eta;
  Bool_t cmva_jet2_gen;
  Float_t cmva_jet2_gen_eta;
  Float_t cmva_jet2_gen_m;
  Int_t cmva_jet2_gen_num_b;
  Int_t cmva_jet2_gen_parton_flav;
  Int_t cmva_jet2_gen_pdgid;
  Float_t cmva_jet2_gen_phi;
  Float_t cmva_jet2_gen_pt;
  Float_t cmva_jet2_leadlep_dr;
  Float_t cmva_jet2_leadlep_pt;
  Float_t cmva_jet2_leadlep_ptrel;
  Float_t cmva_jet2_loose_sf;
  Float_t cmva_jet2_loose_sf_down;
  Float_t cmva_jet2_loose_sf_up;
  Float_t cmva_jet2_m;
  Float_t cmva_jet2_maxtrk;
  Float_t cmva_jet2_medium_sf;
  Float_t cmva_jet2_medium_sf_down;
  Float_t cmva_jet2_medium_sf_up;
  Float_t cmva_jet2_nhf;
  Int_t cmva_jet2_nlep;
  Float_t cmva_jet2_phi;
  Float_t cmva_jet2_pt;
  Float_t cmva_jet2_pt_ratio;
  Float_t cmva_jet2_pt_reg;
  Float_t cmva_jet2_qgl;
  Float_t cmva_jet2_tight_sf;
  Float_t cmva_jet2_tight_sf_down;
  Float_t cmva_jet2_tight_sf_up;
  Float_t cmva_jet2_vtx_3Derr;
  Float_t cmva_jet2_vtx_3Dval;
  Float_t cmva_jet2_vtx_m;
  Int_t cmva_jet2_vtx_ntrk;
  Float_t cmva_jet2_vtx_pt;
  Bool_t csv_hbb;
  Float_t csv_hbb_eta;
  Float_t csv_hbb_m;
  Float_t csv_hbb_phi;
  Float_t csv_hbb_pt;
  Bool_t csv_jet1;
  Float_t csv_jet1_chf;
  Float_t csv_jet1_cmva;
  Float_t csv_jet1_csv;
  Float_t csv_jet1_deepcmvab;
  Float_t csv_jet1_deepcsvb;
  Float_t csv_jet1_emfrac;
  Float_t csv_jet1_eta;
  Bool_t csv_jet1_gen;
  Float_t csv_jet1_gen_eta;
  Float_t csv_jet1_gen_m;
  Int_t csv_jet1_gen_num_b;
  Int_t csv_jet1_gen_parton_flav;
  Int_t csv_jet1_gen_pdgid;
  Float_t csv_jet1_gen_phi;
  Float_t csv_jet1_gen_pt;
  Float_t csv_jet1_leadlep_dr;
  Float_t csv_jet1_leadlep_pt;
  Float_t csv_jet1_leadlep_ptrel;
  Float_t csv_jet1_loose_sf;
  Float_t csv_jet1_loose_sf_down;
  Float_t csv_jet1_loose_sf_up;
  Float_t csv_jet1_m;
  Float_t csv_jet1_maxtrk;
  Float_t csv_jet1_medium_sf;
  Float_t csv_jet1_medium_sf_down;
  Float_t csv_jet1_medium_sf_up;
  Float_t csv_jet1_nhf;
  Int_t csv_jet1_nlep;
  Float_t csv_jet1_phi;
  Float_t csv_jet1_pt;
  Float_t csv_jet1_pt_ratio;
  Float_t csv_jet1_pt_reg;
  Float_t csv_jet1_qgl;
  Float_t csv_jet1_tight_sf;
  Float_t csv_jet1_tight_sf_down;
  Float_t csv_jet1_tight_sf_up;
  Float_t csv_jet1_vtx_3Derr;
  Float_t csv_jet1_vtx_3Dval;
  Float_t csv_jet1_vtx_m;
  Int_t csv_jet1_vtx_ntrk;
  Float_t csv_jet1_vtx_pt;
  Bool_t csv_jet2;
  Float_t csv_jet2_chf;
  Float_t csv_jet2_cmva;
  Float_t csv_jet2_csv;
  Float_t csv_jet2_deepcmvab;
  Float_t csv_jet2_deepcsvb;
  Float_t csv_jet2_emfrac;
  Float_t csv_jet2_eta;
  Bool_t csv_jet2_gen;
  Float_t csv_jet2_gen_eta;
  Float_t csv_jet2_gen_m;
  Int_t csv_jet2_gen_num_b;
  Int_t csv_jet2_gen_parton_flav;
  Int_t csv_jet2_gen_pdgid;
  Float_t csv_jet2_gen_phi;
  Float_t csv_jet2_gen_pt;
  Float_t csv_jet2_leadlep_dr;
  Float_t csv_jet2_leadlep_pt;
  Float_t csv_jet2_leadlep_ptrel;
  Float_t csv_jet2_loose_sf;
  Float_t csv_jet2_loose_sf_down;
  Float_t csv_jet2_loose_sf_up;
  Float_t csv_jet2_m;
  Float_t csv_jet2_maxtrk;
  Float_t csv_jet2_medium_sf;
  Float_t csv_jet2_medium_sf_down;
  Float_t csv_jet2_medium_sf_up;
  Float_t csv_jet2_nhf;
  Int_t csv_jet2_nlep;
  Float_t csv_jet2_phi;
  Float_t csv_jet2_pt;
  Float_t csv_jet2_pt_ratio;
  Float_t csv_jet2_pt_reg;
  Float_t csv_jet2_qgl;
  Float_t csv_jet2_tight_sf;
  Float_t csv_jet2_tight_sf_down;
  Float_t csv_jet2_tight_sf_up;
  Float_t csv_jet2_vtx_3Derr;
  Float_t csv_jet2_vtx_3Dval;
  Float_t csv_jet2_vtx_m;
  Int_t csv_jet2_vtx_ntrk;
  Float_t csv_jet2_vtx_pt;
  Float_t dphi_met_trkmet;
  Float_t dphi_uh_cmva;
  Float_t dphi_uh_csv;
  Bool_t ele1;
  UChar_t ele1_1l_2m_3t;
  Float_t ele1_eta;
  Float_t ele1_phi;
  Float_t ele1_pt;
  Char_t ele1_q;
  Bool_t ele2;
  UChar_t ele2_1l_2m_3t;
  Float_t ele2_eta;
  Float_t ele2_phi;
  Float_t ele2_pt;
  Char_t ele2_q;
  ULong64_t event_num;
  Bool_t gen_t;
  Float_t gen_t_eta;
  Float_t gen_t_m;
  Int_t gen_t_pdgid;
  Float_t gen_t_phi;
  Float_t gen_t_pt;
  Bool_t gen_tbar;
  Float_t gen_tbar_eta;
  Float_t gen_tbar_m;
  Int_t gen_tbar_pdgid;
  Float_t gen_tbar_phi;
  Float_t gen_tbar_pt;
  Bool_t genboson;
  Float_t genboson_eta;
  Float_t genboson_m;
  Int_t genboson_pdgid;
  Float_t genboson_phi;
  Float_t genboson_pt;
  Bool_t jet1;
  Float_t jet1_chf;
  Float_t jet1_emfrac;
  Float_t jet1_eta;
  Bool_t jet1_gen;
  Float_t jet1_gen_eta;
  Float_t jet1_gen_m;
  Int_t jet1_gen_num_b;
  Int_t jet1_gen_parton_flav;
  Int_t jet1_gen_pdgid;
  Float_t jet1_gen_phi;
  Float_t jet1_gen_pt;
  Float_t jet1_m;
  Float_t jet1_nhf;
  Float_t jet1_phi;
  Float_t jet1_pt;
  Float_t jet1_qgl;
  Bool_t jet2;
  Float_t jet2_chf;
  Float_t jet2_emfrac;
  Float_t jet2_eta;
  Bool_t jet2_gen;
  Float_t jet2_gen_eta;
  Float_t jet2_gen_m;
  Int_t jet2_gen_num_b;
  Int_t jet2_gen_parton_flav;
  Int_t jet2_gen_pdgid;
  Float_t jet2_gen_phi;
  Float_t jet2_gen_pt;
  Float_t jet2_m;
  Float_t jet2_nhf;
  Float_t jet2_phi;
  Float_t jet2_pt;
  Float_t jet2_qgl;
  Bool_t jet3;
  Float_t jet3_chf;
  Float_t jet3_emfrac;
  Float_t jet3_eta;
  Bool_t jet3_gen;
  Float_t jet3_gen_eta;
  Float_t jet3_gen_m;
  Int_t jet3_gen_num_b;
  Int_t jet3_gen_parton_flav;
  Int_t jet3_gen_pdgid;
  Float_t jet3_gen_phi;
  Float_t jet3_gen_pt;
  Float_t jet3_m;
  Float_t jet3_nhf;
  Float_t jet3_phi;
  Float_t jet3_pt;
  Float_t jet3_qgl;
  Int_t lumi_num;
  Float_t mc_weight;
  Float_t met;
  Bool_t met_filter;
  Bool_t met_trigger;
  Float_t metphi;
  Float_t min_dphi_metj_hard;
  Float_t min_dphi_metj_soft;
  Bool_t muon1;
  UChar_t muon1_1l_2m_3t;
  Float_t muon1_eta;
  Float_t muon1_phi;
  Float_t muon1_pt;
  Char_t muon1_q;
  Bool_t muon2;
  UChar_t muon2_1l_2m_3t;
  Float_t muon2_eta;
  Float_t muon2_phi;
  Float_t muon2_pt;
  Char_t muon2_q;
  Int_t n_alljet;
  Int_t n_bcmva_loose;
  Int_t n_bcmva_medium;
  Int_t n_bcmva_tight;
  Int_t n_bcsv_loose;
  Int_t n_bcsv_medium;
  Int_t n_bcsv_tight;
  Int_t n_hardjet;
  Int_t n_jet;
  Int_t n_lep_loose;
  Int_t n_lep_medium;
  Int_t n_lep_tight;
  Int_t n_pho_loose;
  Int_t n_tau_loose;
  Int_t npv;
  Int_t npv_true;
  Float_t recoil;
  Float_t recoilphi;
  Int_t run_num;
  Float_t trkmet;
  Float_t trkmetphi;

  void reset(const panda::Event& event);
  void fill(const TVector2 recoilvec);
  void write(TObject* obj) { f->WriteTObject(obj, obj->GetName()); }
  
  enum class lep : unsigned {
    muon1 = 0,
    muon2 = 1,
    ele1 = 2,
    ele2 = 3
  };
 private:
  const std::vector<std::string> lep_names = {
    "muon1",
    "muon2",
    "ele1",
    "ele2"
  };
 public:
  void set_lep(const lep base, const panda::Lepton& lep, const unsigned char flag);
  
  enum class bjet : unsigned {
    csv_jet1 = 0,
    csv_jet2 = 1,
    cmva_jet1 = 2,
    cmva_jet2 = 3
  };
 private:
  const std::vector<std::string> bjet_names = {
    "csv_jet1",
    "csv_jet2",
    "cmva_jet1",
    "cmva_jet2"
  };
 public:
  void set_bjet(const bjet base, const panda::Jet& jet, const float maxpt, const BCalReaders& readers, const BTagEntry::JetFlavor flav);
  void set_bvert(const bjet base, const panda::SecondaryVertex& vert);
  void set_bleps(const bjet base, const panda::Jet& jet, const int nlep, const panda::PFCand& lep);
  void set_pt_reg(const bjet base);
  
  enum class jet : unsigned {
    csv_jet1 = 0,
    csv_jet2 = 1,
    cmva_jet1 = 2,
    cmva_jet2 = 3,
    jet1 = 4,
    jet2 = 5,
    jet3 = 6
  };
 private:
  const std::vector<std::string> jet_names = {
    "csv_jet1",
    "csv_jet2",
    "cmva_jet1",
    "cmva_jet2",
    "jet1",
    "jet2",
    "jet3"
  };
 public:
  void set_jet(const jet base, const panda::Jet& jet);
  void set_genjet(const jet base, const panda::GenJet& gen);
  
  enum class hbb : unsigned {
    csv_hbb = 0,
    cmva_hbb = 1
  };
 private:
  const std::vector<std::string> hbb_names = {
    "csv_hbb",
    "cmva_hbb"
  };
 public:
  void set_hbb(const hbb base, const TLorentzVector& vec);
  
  enum class gen : unsigned {
    genboson = 0,
    gen_t = 1,
    gen_tbar = 2
  };
 private:
  const std::vector<std::string> gen_names = {
    "genboson",
    "gen_t",
    "gen_tbar"
  };
 public:
  void set_gen(const gen base, const panda::GenParticle& gen);

 private:
  TFile* f;
  TTree* t;  

  template <typename T>
  void set(std::string name, T val) { *(T*)(t->GetBranch(name.data())->GetAddress()) = val; }

  const std::unordered_map<std::string, void*> addresses {
    {"calomet", &calomet},
    {"calometphi", &calometphi},
    {"cmva_hbb", &cmva_hbb},
    {"cmva_hbb_eta", &cmva_hbb_eta},
    {"cmva_hbb_m", &cmva_hbb_m},
    {"cmva_hbb_phi", &cmva_hbb_phi},
    {"cmva_hbb_pt", &cmva_hbb_pt},
    {"cmva_jet1", &cmva_jet1},
    {"cmva_jet1_chf", &cmva_jet1_chf},
    {"cmva_jet1_cmva", &cmva_jet1_cmva},
    {"cmva_jet1_csv", &cmva_jet1_csv},
    {"cmva_jet1_deepcmvab", &cmva_jet1_deepcmvab},
    {"cmva_jet1_deepcsvb", &cmva_jet1_deepcsvb},
    {"cmva_jet1_emfrac", &cmva_jet1_emfrac},
    {"cmva_jet1_eta", &cmva_jet1_eta},
    {"cmva_jet1_gen", &cmva_jet1_gen},
    {"cmva_jet1_gen_eta", &cmva_jet1_gen_eta},
    {"cmva_jet1_gen_m", &cmva_jet1_gen_m},
    {"cmva_jet1_gen_num_b", &cmva_jet1_gen_num_b},
    {"cmva_jet1_gen_parton_flav", &cmva_jet1_gen_parton_flav},
    {"cmva_jet1_gen_pdgid", &cmva_jet1_gen_pdgid},
    {"cmva_jet1_gen_phi", &cmva_jet1_gen_phi},
    {"cmva_jet1_gen_pt", &cmva_jet1_gen_pt},
    {"cmva_jet1_leadlep_dr", &cmva_jet1_leadlep_dr},
    {"cmva_jet1_leadlep_pt", &cmva_jet1_leadlep_pt},
    {"cmva_jet1_leadlep_ptrel", &cmva_jet1_leadlep_ptrel},
    {"cmva_jet1_loose_sf", &cmva_jet1_loose_sf},
    {"cmva_jet1_loose_sf_down", &cmva_jet1_loose_sf_down},
    {"cmva_jet1_loose_sf_up", &cmva_jet1_loose_sf_up},
    {"cmva_jet1_m", &cmva_jet1_m},
    {"cmva_jet1_maxtrk", &cmva_jet1_maxtrk},
    {"cmva_jet1_medium_sf", &cmva_jet1_medium_sf},
    {"cmva_jet1_medium_sf_down", &cmva_jet1_medium_sf_down},
    {"cmva_jet1_medium_sf_up", &cmva_jet1_medium_sf_up},
    {"cmva_jet1_nhf", &cmva_jet1_nhf},
    {"cmva_jet1_nlep", &cmva_jet1_nlep},
    {"cmva_jet1_phi", &cmva_jet1_phi},
    {"cmva_jet1_pt", &cmva_jet1_pt},
    {"cmva_jet1_pt_ratio", &cmva_jet1_pt_ratio},
    {"cmva_jet1_pt_reg", &cmva_jet1_pt_reg},
    {"cmva_jet1_qgl", &cmva_jet1_qgl},
    {"cmva_jet1_tight_sf", &cmva_jet1_tight_sf},
    {"cmva_jet1_tight_sf_down", &cmva_jet1_tight_sf_down},
    {"cmva_jet1_tight_sf_up", &cmva_jet1_tight_sf_up},
    {"cmva_jet1_vtx_3Derr", &cmva_jet1_vtx_3Derr},
    {"cmva_jet1_vtx_3Dval", &cmva_jet1_vtx_3Dval},
    {"cmva_jet1_vtx_m", &cmva_jet1_vtx_m},
    {"cmva_jet1_vtx_ntrk", &cmva_jet1_vtx_ntrk},
    {"cmva_jet1_vtx_pt", &cmva_jet1_vtx_pt},
    {"cmva_jet2", &cmva_jet2},
    {"cmva_jet2_chf", &cmva_jet2_chf},
    {"cmva_jet2_cmva", &cmva_jet2_cmva},
    {"cmva_jet2_csv", &cmva_jet2_csv},
    {"cmva_jet2_deepcmvab", &cmva_jet2_deepcmvab},
    {"cmva_jet2_deepcsvb", &cmva_jet2_deepcsvb},
    {"cmva_jet2_emfrac", &cmva_jet2_emfrac},
    {"cmva_jet2_eta", &cmva_jet2_eta},
    {"cmva_jet2_gen", &cmva_jet2_gen},
    {"cmva_jet2_gen_eta", &cmva_jet2_gen_eta},
    {"cmva_jet2_gen_m", &cmva_jet2_gen_m},
    {"cmva_jet2_gen_num_b", &cmva_jet2_gen_num_b},
    {"cmva_jet2_gen_parton_flav", &cmva_jet2_gen_parton_flav},
    {"cmva_jet2_gen_pdgid", &cmva_jet2_gen_pdgid},
    {"cmva_jet2_gen_phi", &cmva_jet2_gen_phi},
    {"cmva_jet2_gen_pt", &cmva_jet2_gen_pt},
    {"cmva_jet2_leadlep_dr", &cmva_jet2_leadlep_dr},
    {"cmva_jet2_leadlep_pt", &cmva_jet2_leadlep_pt},
    {"cmva_jet2_leadlep_ptrel", &cmva_jet2_leadlep_ptrel},
    {"cmva_jet2_loose_sf", &cmva_jet2_loose_sf},
    {"cmva_jet2_loose_sf_down", &cmva_jet2_loose_sf_down},
    {"cmva_jet2_loose_sf_up", &cmva_jet2_loose_sf_up},
    {"cmva_jet2_m", &cmva_jet2_m},
    {"cmva_jet2_maxtrk", &cmva_jet2_maxtrk},
    {"cmva_jet2_medium_sf", &cmva_jet2_medium_sf},
    {"cmva_jet2_medium_sf_down", &cmva_jet2_medium_sf_down},
    {"cmva_jet2_medium_sf_up", &cmva_jet2_medium_sf_up},
    {"cmva_jet2_nhf", &cmva_jet2_nhf},
    {"cmva_jet2_nlep", &cmva_jet2_nlep},
    {"cmva_jet2_phi", &cmva_jet2_phi},
    {"cmva_jet2_pt", &cmva_jet2_pt},
    {"cmva_jet2_pt_ratio", &cmva_jet2_pt_ratio},
    {"cmva_jet2_pt_reg", &cmva_jet2_pt_reg},
    {"cmva_jet2_qgl", &cmva_jet2_qgl},
    {"cmva_jet2_tight_sf", &cmva_jet2_tight_sf},
    {"cmva_jet2_tight_sf_down", &cmva_jet2_tight_sf_down},
    {"cmva_jet2_tight_sf_up", &cmva_jet2_tight_sf_up},
    {"cmva_jet2_vtx_3Derr", &cmva_jet2_vtx_3Derr},
    {"cmva_jet2_vtx_3Dval", &cmva_jet2_vtx_3Dval},
    {"cmva_jet2_vtx_m", &cmva_jet2_vtx_m},
    {"cmva_jet2_vtx_ntrk", &cmva_jet2_vtx_ntrk},
    {"cmva_jet2_vtx_pt", &cmva_jet2_vtx_pt},
    {"csv_hbb", &csv_hbb},
    {"csv_hbb_eta", &csv_hbb_eta},
    {"csv_hbb_m", &csv_hbb_m},
    {"csv_hbb_phi", &csv_hbb_phi},
    {"csv_hbb_pt", &csv_hbb_pt},
    {"csv_jet1", &csv_jet1},
    {"csv_jet1_chf", &csv_jet1_chf},
    {"csv_jet1_cmva", &csv_jet1_cmva},
    {"csv_jet1_csv", &csv_jet1_csv},
    {"csv_jet1_deepcmvab", &csv_jet1_deepcmvab},
    {"csv_jet1_deepcsvb", &csv_jet1_deepcsvb},
    {"csv_jet1_emfrac", &csv_jet1_emfrac},
    {"csv_jet1_eta", &csv_jet1_eta},
    {"csv_jet1_gen", &csv_jet1_gen},
    {"csv_jet1_gen_eta", &csv_jet1_gen_eta},
    {"csv_jet1_gen_m", &csv_jet1_gen_m},
    {"csv_jet1_gen_num_b", &csv_jet1_gen_num_b},
    {"csv_jet1_gen_parton_flav", &csv_jet1_gen_parton_flav},
    {"csv_jet1_gen_pdgid", &csv_jet1_gen_pdgid},
    {"csv_jet1_gen_phi", &csv_jet1_gen_phi},
    {"csv_jet1_gen_pt", &csv_jet1_gen_pt},
    {"csv_jet1_leadlep_dr", &csv_jet1_leadlep_dr},
    {"csv_jet1_leadlep_pt", &csv_jet1_leadlep_pt},
    {"csv_jet1_leadlep_ptrel", &csv_jet1_leadlep_ptrel},
    {"csv_jet1_loose_sf", &csv_jet1_loose_sf},
    {"csv_jet1_loose_sf_down", &csv_jet1_loose_sf_down},
    {"csv_jet1_loose_sf_up", &csv_jet1_loose_sf_up},
    {"csv_jet1_m", &csv_jet1_m},
    {"csv_jet1_maxtrk", &csv_jet1_maxtrk},
    {"csv_jet1_medium_sf", &csv_jet1_medium_sf},
    {"csv_jet1_medium_sf_down", &csv_jet1_medium_sf_down},
    {"csv_jet1_medium_sf_up", &csv_jet1_medium_sf_up},
    {"csv_jet1_nhf", &csv_jet1_nhf},
    {"csv_jet1_nlep", &csv_jet1_nlep},
    {"csv_jet1_phi", &csv_jet1_phi},
    {"csv_jet1_pt", &csv_jet1_pt},
    {"csv_jet1_pt_ratio", &csv_jet1_pt_ratio},
    {"csv_jet1_pt_reg", &csv_jet1_pt_reg},
    {"csv_jet1_qgl", &csv_jet1_qgl},
    {"csv_jet1_tight_sf", &csv_jet1_tight_sf},
    {"csv_jet1_tight_sf_down", &csv_jet1_tight_sf_down},
    {"csv_jet1_tight_sf_up", &csv_jet1_tight_sf_up},
    {"csv_jet1_vtx_3Derr", &csv_jet1_vtx_3Derr},
    {"csv_jet1_vtx_3Dval", &csv_jet1_vtx_3Dval},
    {"csv_jet1_vtx_m", &csv_jet1_vtx_m},
    {"csv_jet1_vtx_ntrk", &csv_jet1_vtx_ntrk},
    {"csv_jet1_vtx_pt", &csv_jet1_vtx_pt},
    {"csv_jet2", &csv_jet2},
    {"csv_jet2_chf", &csv_jet2_chf},
    {"csv_jet2_cmva", &csv_jet2_cmva},
    {"csv_jet2_csv", &csv_jet2_csv},
    {"csv_jet2_deepcmvab", &csv_jet2_deepcmvab},
    {"csv_jet2_deepcsvb", &csv_jet2_deepcsvb},
    {"csv_jet2_emfrac", &csv_jet2_emfrac},
    {"csv_jet2_eta", &csv_jet2_eta},
    {"csv_jet2_gen", &csv_jet2_gen},
    {"csv_jet2_gen_eta", &csv_jet2_gen_eta},
    {"csv_jet2_gen_m", &csv_jet2_gen_m},
    {"csv_jet2_gen_num_b", &csv_jet2_gen_num_b},
    {"csv_jet2_gen_parton_flav", &csv_jet2_gen_parton_flav},
    {"csv_jet2_gen_pdgid", &csv_jet2_gen_pdgid},
    {"csv_jet2_gen_phi", &csv_jet2_gen_phi},
    {"csv_jet2_gen_pt", &csv_jet2_gen_pt},
    {"csv_jet2_leadlep_dr", &csv_jet2_leadlep_dr},
    {"csv_jet2_leadlep_pt", &csv_jet2_leadlep_pt},
    {"csv_jet2_leadlep_ptrel", &csv_jet2_leadlep_ptrel},
    {"csv_jet2_loose_sf", &csv_jet2_loose_sf},
    {"csv_jet2_loose_sf_down", &csv_jet2_loose_sf_down},
    {"csv_jet2_loose_sf_up", &csv_jet2_loose_sf_up},
    {"csv_jet2_m", &csv_jet2_m},
    {"csv_jet2_maxtrk", &csv_jet2_maxtrk},
    {"csv_jet2_medium_sf", &csv_jet2_medium_sf},
    {"csv_jet2_medium_sf_down", &csv_jet2_medium_sf_down},
    {"csv_jet2_medium_sf_up", &csv_jet2_medium_sf_up},
    {"csv_jet2_nhf", &csv_jet2_nhf},
    {"csv_jet2_nlep", &csv_jet2_nlep},
    {"csv_jet2_phi", &csv_jet2_phi},
    {"csv_jet2_pt", &csv_jet2_pt},
    {"csv_jet2_pt_ratio", &csv_jet2_pt_ratio},
    {"csv_jet2_pt_reg", &csv_jet2_pt_reg},
    {"csv_jet2_qgl", &csv_jet2_qgl},
    {"csv_jet2_tight_sf", &csv_jet2_tight_sf},
    {"csv_jet2_tight_sf_down", &csv_jet2_tight_sf_down},
    {"csv_jet2_tight_sf_up", &csv_jet2_tight_sf_up},
    {"csv_jet2_vtx_3Derr", &csv_jet2_vtx_3Derr},
    {"csv_jet2_vtx_3Dval", &csv_jet2_vtx_3Dval},
    {"csv_jet2_vtx_m", &csv_jet2_vtx_m},
    {"csv_jet2_vtx_ntrk", &csv_jet2_vtx_ntrk},
    {"csv_jet2_vtx_pt", &csv_jet2_vtx_pt},
    {"dphi_met_trkmet", &dphi_met_trkmet},
    {"dphi_uh_cmva", &dphi_uh_cmva},
    {"dphi_uh_csv", &dphi_uh_csv},
    {"ele1", &ele1},
    {"ele1_1l_2m_3t", &ele1_1l_2m_3t},
    {"ele1_eta", &ele1_eta},
    {"ele1_phi", &ele1_phi},
    {"ele1_pt", &ele1_pt},
    {"ele1_q", &ele1_q},
    {"ele2", &ele2},
    {"ele2_1l_2m_3t", &ele2_1l_2m_3t},
    {"ele2_eta", &ele2_eta},
    {"ele2_phi", &ele2_phi},
    {"ele2_pt", &ele2_pt},
    {"ele2_q", &ele2_q},
    {"event_num", &event_num},
    {"gen_t", &gen_t},
    {"gen_t_eta", &gen_t_eta},
    {"gen_t_m", &gen_t_m},
    {"gen_t_pdgid", &gen_t_pdgid},
    {"gen_t_phi", &gen_t_phi},
    {"gen_t_pt", &gen_t_pt},
    {"gen_tbar", &gen_tbar},
    {"gen_tbar_eta", &gen_tbar_eta},
    {"gen_tbar_m", &gen_tbar_m},
    {"gen_tbar_pdgid", &gen_tbar_pdgid},
    {"gen_tbar_phi", &gen_tbar_phi},
    {"gen_tbar_pt", &gen_tbar_pt},
    {"genboson", &genboson},
    {"genboson_eta", &genboson_eta},
    {"genboson_m", &genboson_m},
    {"genboson_pdgid", &genboson_pdgid},
    {"genboson_phi", &genboson_phi},
    {"genboson_pt", &genboson_pt},
    {"jet1", &jet1},
    {"jet1_chf", &jet1_chf},
    {"jet1_emfrac", &jet1_emfrac},
    {"jet1_eta", &jet1_eta},
    {"jet1_gen", &jet1_gen},
    {"jet1_gen_eta", &jet1_gen_eta},
    {"jet1_gen_m", &jet1_gen_m},
    {"jet1_gen_num_b", &jet1_gen_num_b},
    {"jet1_gen_parton_flav", &jet1_gen_parton_flav},
    {"jet1_gen_pdgid", &jet1_gen_pdgid},
    {"jet1_gen_phi", &jet1_gen_phi},
    {"jet1_gen_pt", &jet1_gen_pt},
    {"jet1_m", &jet1_m},
    {"jet1_nhf", &jet1_nhf},
    {"jet1_phi", &jet1_phi},
    {"jet1_pt", &jet1_pt},
    {"jet1_qgl", &jet1_qgl},
    {"jet2", &jet2},
    {"jet2_chf", &jet2_chf},
    {"jet2_emfrac", &jet2_emfrac},
    {"jet2_eta", &jet2_eta},
    {"jet2_gen", &jet2_gen},
    {"jet2_gen_eta", &jet2_gen_eta},
    {"jet2_gen_m", &jet2_gen_m},
    {"jet2_gen_num_b", &jet2_gen_num_b},
    {"jet2_gen_parton_flav", &jet2_gen_parton_flav},
    {"jet2_gen_pdgid", &jet2_gen_pdgid},
    {"jet2_gen_phi", &jet2_gen_phi},
    {"jet2_gen_pt", &jet2_gen_pt},
    {"jet2_m", &jet2_m},
    {"jet2_nhf", &jet2_nhf},
    {"jet2_phi", &jet2_phi},
    {"jet2_pt", &jet2_pt},
    {"jet2_qgl", &jet2_qgl},
    {"jet3", &jet3},
    {"jet3_chf", &jet3_chf},
    {"jet3_emfrac", &jet3_emfrac},
    {"jet3_eta", &jet3_eta},
    {"jet3_gen", &jet3_gen},
    {"jet3_gen_eta", &jet3_gen_eta},
    {"jet3_gen_m", &jet3_gen_m},
    {"jet3_gen_num_b", &jet3_gen_num_b},
    {"jet3_gen_parton_flav", &jet3_gen_parton_flav},
    {"jet3_gen_pdgid", &jet3_gen_pdgid},
    {"jet3_gen_phi", &jet3_gen_phi},
    {"jet3_gen_pt", &jet3_gen_pt},
    {"jet3_m", &jet3_m},
    {"jet3_nhf", &jet3_nhf},
    {"jet3_phi", &jet3_phi},
    {"jet3_pt", &jet3_pt},
    {"jet3_qgl", &jet3_qgl},
    {"lumi_num", &lumi_num},
    {"mc_weight", &mc_weight},
    {"met", &met},
    {"met_filter", &met_filter},
    {"met_trigger", &met_trigger},
    {"metphi", &metphi},
    {"min_dphi_metj_hard", &min_dphi_metj_hard},
    {"min_dphi_metj_soft", &min_dphi_metj_soft},
    {"muon1", &muon1},
    {"muon1_1l_2m_3t", &muon1_1l_2m_3t},
    {"muon1_eta", &muon1_eta},
    {"muon1_phi", &muon1_phi},
    {"muon1_pt", &muon1_pt},
    {"muon1_q", &muon1_q},
    {"muon2", &muon2},
    {"muon2_1l_2m_3t", &muon2_1l_2m_3t},
    {"muon2_eta", &muon2_eta},
    {"muon2_phi", &muon2_phi},
    {"muon2_pt", &muon2_pt},
    {"muon2_q", &muon2_q},
    {"n_alljet", &n_alljet},
    {"n_bcmva_loose", &n_bcmva_loose},
    {"n_bcmva_medium", &n_bcmva_medium},
    {"n_bcmva_tight", &n_bcmva_tight},
    {"n_bcsv_loose", &n_bcsv_loose},
    {"n_bcsv_medium", &n_bcsv_medium},
    {"n_bcsv_tight", &n_bcsv_tight},
    {"n_hardjet", &n_hardjet},
    {"n_jet", &n_jet},
    {"n_lep_loose", &n_lep_loose},
    {"n_lep_medium", &n_lep_medium},
    {"n_lep_tight", &n_lep_tight},
    {"n_pho_loose", &n_pho_loose},
    {"n_tau_loose", &n_tau_loose},
    {"npv", &npv},
    {"npv_true", &npv_true},
    {"recoil", &recoil},
    {"recoilphi", &recoilphi},
    {"run_num", &run_num},
    {"trkmet", &trkmet},
    {"trkmetphi", &trkmetphi}
  };
};

hbbfile::hbbfile(const char* outfile_name, const char* name) {
  f = new TFile(outfile_name, "CREATE");
  t = new TTree(name, name);
  t->Branch("calomet", &calomet, "calomet/F");
  t->Branch("calometphi", &calometphi, "calometphi/F");
  t->Branch("cmva_hbb", &cmva_hbb, "cmva_hbb/O");
  t->Branch("cmva_hbb_eta", &cmva_hbb_eta, "cmva_hbb_eta/F");
  t->Branch("cmva_hbb_m", &cmva_hbb_m, "cmva_hbb_m/F");
  t->Branch("cmva_hbb_phi", &cmva_hbb_phi, "cmva_hbb_phi/F");
  t->Branch("cmva_hbb_pt", &cmva_hbb_pt, "cmva_hbb_pt/F");
  t->Branch("cmva_jet1", &cmva_jet1, "cmva_jet1/O");
  t->Branch("cmva_jet1_chf", &cmva_jet1_chf, "cmva_jet1_chf/F");
  t->Branch("cmva_jet1_cmva", &cmva_jet1_cmva, "cmva_jet1_cmva/F");
  t->Branch("cmva_jet1_csv", &cmva_jet1_csv, "cmva_jet1_csv/F");
  t->Branch("cmva_jet1_deepcmvab", &cmva_jet1_deepcmvab, "cmva_jet1_deepcmvab/F");
  t->Branch("cmva_jet1_deepcsvb", &cmva_jet1_deepcsvb, "cmva_jet1_deepcsvb/F");
  t->Branch("cmva_jet1_emfrac", &cmva_jet1_emfrac, "cmva_jet1_emfrac/F");
  t->Branch("cmva_jet1_eta", &cmva_jet1_eta, "cmva_jet1_eta/F");
  t->Branch("cmva_jet1_gen", &cmva_jet1_gen, "cmva_jet1_gen/O");
  t->Branch("cmva_jet1_gen_eta", &cmva_jet1_gen_eta, "cmva_jet1_gen_eta/F");
  t->Branch("cmva_jet1_gen_m", &cmva_jet1_gen_m, "cmva_jet1_gen_m/F");
  t->Branch("cmva_jet1_gen_num_b", &cmva_jet1_gen_num_b, "cmva_jet1_gen_num_b/I");
  t->Branch("cmva_jet1_gen_parton_flav", &cmva_jet1_gen_parton_flav, "cmva_jet1_gen_parton_flav/I");
  t->Branch("cmva_jet1_gen_pdgid", &cmva_jet1_gen_pdgid, "cmva_jet1_gen_pdgid/I");
  t->Branch("cmva_jet1_gen_phi", &cmva_jet1_gen_phi, "cmva_jet1_gen_phi/F");
  t->Branch("cmva_jet1_gen_pt", &cmva_jet1_gen_pt, "cmva_jet1_gen_pt/F");
  t->Branch("cmva_jet1_leadlep_dr", &cmva_jet1_leadlep_dr, "cmva_jet1_leadlep_dr/F");
  t->Branch("cmva_jet1_leadlep_pt", &cmva_jet1_leadlep_pt, "cmva_jet1_leadlep_pt/F");
  t->Branch("cmva_jet1_leadlep_ptrel", &cmva_jet1_leadlep_ptrel, "cmva_jet1_leadlep_ptrel/F");
  t->Branch("cmva_jet1_loose_sf", &cmva_jet1_loose_sf, "cmva_jet1_loose_sf/F");
  t->Branch("cmva_jet1_loose_sf_down", &cmva_jet1_loose_sf_down, "cmva_jet1_loose_sf_down/F");
  t->Branch("cmva_jet1_loose_sf_up", &cmva_jet1_loose_sf_up, "cmva_jet1_loose_sf_up/F");
  t->Branch("cmva_jet1_m", &cmva_jet1_m, "cmva_jet1_m/F");
  t->Branch("cmva_jet1_maxtrk", &cmva_jet1_maxtrk, "cmva_jet1_maxtrk/F");
  t->Branch("cmva_jet1_medium_sf", &cmva_jet1_medium_sf, "cmva_jet1_medium_sf/F");
  t->Branch("cmva_jet1_medium_sf_down", &cmva_jet1_medium_sf_down, "cmva_jet1_medium_sf_down/F");
  t->Branch("cmva_jet1_medium_sf_up", &cmva_jet1_medium_sf_up, "cmva_jet1_medium_sf_up/F");
  t->Branch("cmva_jet1_nhf", &cmva_jet1_nhf, "cmva_jet1_nhf/F");
  t->Branch("cmva_jet1_nlep", &cmva_jet1_nlep, "cmva_jet1_nlep/I");
  t->Branch("cmva_jet1_phi", &cmva_jet1_phi, "cmva_jet1_phi/F");
  t->Branch("cmva_jet1_pt", &cmva_jet1_pt, "cmva_jet1_pt/F");
  t->Branch("cmva_jet1_pt_ratio", &cmva_jet1_pt_ratio, "cmva_jet1_pt_ratio/F");
  t->Branch("cmva_jet1_pt_reg", &cmva_jet1_pt_reg, "cmva_jet1_pt_reg/F");
  t->Branch("cmva_jet1_qgl", &cmva_jet1_qgl, "cmva_jet1_qgl/F");
  t->Branch("cmva_jet1_tight_sf", &cmva_jet1_tight_sf, "cmva_jet1_tight_sf/F");
  t->Branch("cmva_jet1_tight_sf_down", &cmva_jet1_tight_sf_down, "cmva_jet1_tight_sf_down/F");
  t->Branch("cmva_jet1_tight_sf_up", &cmva_jet1_tight_sf_up, "cmva_jet1_tight_sf_up/F");
  t->Branch("cmva_jet1_vtx_3Derr", &cmva_jet1_vtx_3Derr, "cmva_jet1_vtx_3Derr/F");
  t->Branch("cmva_jet1_vtx_3Dval", &cmva_jet1_vtx_3Dval, "cmva_jet1_vtx_3Dval/F");
  t->Branch("cmva_jet1_vtx_m", &cmva_jet1_vtx_m, "cmva_jet1_vtx_m/F");
  t->Branch("cmva_jet1_vtx_ntrk", &cmva_jet1_vtx_ntrk, "cmva_jet1_vtx_ntrk/I");
  t->Branch("cmva_jet1_vtx_pt", &cmva_jet1_vtx_pt, "cmva_jet1_vtx_pt/F");
  t->Branch("cmva_jet2", &cmva_jet2, "cmva_jet2/O");
  t->Branch("cmva_jet2_chf", &cmva_jet2_chf, "cmva_jet2_chf/F");
  t->Branch("cmva_jet2_cmva", &cmva_jet2_cmva, "cmva_jet2_cmva/F");
  t->Branch("cmva_jet2_csv", &cmva_jet2_csv, "cmva_jet2_csv/F");
  t->Branch("cmva_jet2_deepcmvab", &cmva_jet2_deepcmvab, "cmva_jet2_deepcmvab/F");
  t->Branch("cmva_jet2_deepcsvb", &cmva_jet2_deepcsvb, "cmva_jet2_deepcsvb/F");
  t->Branch("cmva_jet2_emfrac", &cmva_jet2_emfrac, "cmva_jet2_emfrac/F");
  t->Branch("cmva_jet2_eta", &cmva_jet2_eta, "cmva_jet2_eta/F");
  t->Branch("cmva_jet2_gen", &cmva_jet2_gen, "cmva_jet2_gen/O");
  t->Branch("cmva_jet2_gen_eta", &cmva_jet2_gen_eta, "cmva_jet2_gen_eta/F");
  t->Branch("cmva_jet2_gen_m", &cmva_jet2_gen_m, "cmva_jet2_gen_m/F");
  t->Branch("cmva_jet2_gen_num_b", &cmva_jet2_gen_num_b, "cmva_jet2_gen_num_b/I");
  t->Branch("cmva_jet2_gen_parton_flav", &cmva_jet2_gen_parton_flav, "cmva_jet2_gen_parton_flav/I");
  t->Branch("cmva_jet2_gen_pdgid", &cmva_jet2_gen_pdgid, "cmva_jet2_gen_pdgid/I");
  t->Branch("cmva_jet2_gen_phi", &cmva_jet2_gen_phi, "cmva_jet2_gen_phi/F");
  t->Branch("cmva_jet2_gen_pt", &cmva_jet2_gen_pt, "cmva_jet2_gen_pt/F");
  t->Branch("cmva_jet2_leadlep_dr", &cmva_jet2_leadlep_dr, "cmva_jet2_leadlep_dr/F");
  t->Branch("cmva_jet2_leadlep_pt", &cmva_jet2_leadlep_pt, "cmva_jet2_leadlep_pt/F");
  t->Branch("cmva_jet2_leadlep_ptrel", &cmva_jet2_leadlep_ptrel, "cmva_jet2_leadlep_ptrel/F");
  t->Branch("cmva_jet2_loose_sf", &cmva_jet2_loose_sf, "cmva_jet2_loose_sf/F");
  t->Branch("cmva_jet2_loose_sf_down", &cmva_jet2_loose_sf_down, "cmva_jet2_loose_sf_down/F");
  t->Branch("cmva_jet2_loose_sf_up", &cmva_jet2_loose_sf_up, "cmva_jet2_loose_sf_up/F");
  t->Branch("cmva_jet2_m", &cmva_jet2_m, "cmva_jet2_m/F");
  t->Branch("cmva_jet2_maxtrk", &cmva_jet2_maxtrk, "cmva_jet2_maxtrk/F");
  t->Branch("cmva_jet2_medium_sf", &cmva_jet2_medium_sf, "cmva_jet2_medium_sf/F");
  t->Branch("cmva_jet2_medium_sf_down", &cmva_jet2_medium_sf_down, "cmva_jet2_medium_sf_down/F");
  t->Branch("cmva_jet2_medium_sf_up", &cmva_jet2_medium_sf_up, "cmva_jet2_medium_sf_up/F");
  t->Branch("cmva_jet2_nhf", &cmva_jet2_nhf, "cmva_jet2_nhf/F");
  t->Branch("cmva_jet2_nlep", &cmva_jet2_nlep, "cmva_jet2_nlep/I");
  t->Branch("cmva_jet2_phi", &cmva_jet2_phi, "cmva_jet2_phi/F");
  t->Branch("cmva_jet2_pt", &cmva_jet2_pt, "cmva_jet2_pt/F");
  t->Branch("cmva_jet2_pt_ratio", &cmva_jet2_pt_ratio, "cmva_jet2_pt_ratio/F");
  t->Branch("cmva_jet2_pt_reg", &cmva_jet2_pt_reg, "cmva_jet2_pt_reg/F");
  t->Branch("cmva_jet2_qgl", &cmva_jet2_qgl, "cmva_jet2_qgl/F");
  t->Branch("cmva_jet2_tight_sf", &cmva_jet2_tight_sf, "cmva_jet2_tight_sf/F");
  t->Branch("cmva_jet2_tight_sf_down", &cmva_jet2_tight_sf_down, "cmva_jet2_tight_sf_down/F");
  t->Branch("cmva_jet2_tight_sf_up", &cmva_jet2_tight_sf_up, "cmva_jet2_tight_sf_up/F");
  t->Branch("cmva_jet2_vtx_3Derr", &cmva_jet2_vtx_3Derr, "cmva_jet2_vtx_3Derr/F");
  t->Branch("cmva_jet2_vtx_3Dval", &cmva_jet2_vtx_3Dval, "cmva_jet2_vtx_3Dval/F");
  t->Branch("cmva_jet2_vtx_m", &cmva_jet2_vtx_m, "cmva_jet2_vtx_m/F");
  t->Branch("cmva_jet2_vtx_ntrk", &cmva_jet2_vtx_ntrk, "cmva_jet2_vtx_ntrk/I");
  t->Branch("cmva_jet2_vtx_pt", &cmva_jet2_vtx_pt, "cmva_jet2_vtx_pt/F");
  t->Branch("csv_hbb", &csv_hbb, "csv_hbb/O");
  t->Branch("csv_hbb_eta", &csv_hbb_eta, "csv_hbb_eta/F");
  t->Branch("csv_hbb_m", &csv_hbb_m, "csv_hbb_m/F");
  t->Branch("csv_hbb_phi", &csv_hbb_phi, "csv_hbb_phi/F");
  t->Branch("csv_hbb_pt", &csv_hbb_pt, "csv_hbb_pt/F");
  t->Branch("csv_jet1", &csv_jet1, "csv_jet1/O");
  t->Branch("csv_jet1_chf", &csv_jet1_chf, "csv_jet1_chf/F");
  t->Branch("csv_jet1_cmva", &csv_jet1_cmva, "csv_jet1_cmva/F");
  t->Branch("csv_jet1_csv", &csv_jet1_csv, "csv_jet1_csv/F");
  t->Branch("csv_jet1_deepcmvab", &csv_jet1_deepcmvab, "csv_jet1_deepcmvab/F");
  t->Branch("csv_jet1_deepcsvb", &csv_jet1_deepcsvb, "csv_jet1_deepcsvb/F");
  t->Branch("csv_jet1_emfrac", &csv_jet1_emfrac, "csv_jet1_emfrac/F");
  t->Branch("csv_jet1_eta", &csv_jet1_eta, "csv_jet1_eta/F");
  t->Branch("csv_jet1_gen", &csv_jet1_gen, "csv_jet1_gen/O");
  t->Branch("csv_jet1_gen_eta", &csv_jet1_gen_eta, "csv_jet1_gen_eta/F");
  t->Branch("csv_jet1_gen_m", &csv_jet1_gen_m, "csv_jet1_gen_m/F");
  t->Branch("csv_jet1_gen_num_b", &csv_jet1_gen_num_b, "csv_jet1_gen_num_b/I");
  t->Branch("csv_jet1_gen_parton_flav", &csv_jet1_gen_parton_flav, "csv_jet1_gen_parton_flav/I");
  t->Branch("csv_jet1_gen_pdgid", &csv_jet1_gen_pdgid, "csv_jet1_gen_pdgid/I");
  t->Branch("csv_jet1_gen_phi", &csv_jet1_gen_phi, "csv_jet1_gen_phi/F");
  t->Branch("csv_jet1_gen_pt", &csv_jet1_gen_pt, "csv_jet1_gen_pt/F");
  t->Branch("csv_jet1_leadlep_dr", &csv_jet1_leadlep_dr, "csv_jet1_leadlep_dr/F");
  t->Branch("csv_jet1_leadlep_pt", &csv_jet1_leadlep_pt, "csv_jet1_leadlep_pt/F");
  t->Branch("csv_jet1_leadlep_ptrel", &csv_jet1_leadlep_ptrel, "csv_jet1_leadlep_ptrel/F");
  t->Branch("csv_jet1_loose_sf", &csv_jet1_loose_sf, "csv_jet1_loose_sf/F");
  t->Branch("csv_jet1_loose_sf_down", &csv_jet1_loose_sf_down, "csv_jet1_loose_sf_down/F");
  t->Branch("csv_jet1_loose_sf_up", &csv_jet1_loose_sf_up, "csv_jet1_loose_sf_up/F");
  t->Branch("csv_jet1_m", &csv_jet1_m, "csv_jet1_m/F");
  t->Branch("csv_jet1_maxtrk", &csv_jet1_maxtrk, "csv_jet1_maxtrk/F");
  t->Branch("csv_jet1_medium_sf", &csv_jet1_medium_sf, "csv_jet1_medium_sf/F");
  t->Branch("csv_jet1_medium_sf_down", &csv_jet1_medium_sf_down, "csv_jet1_medium_sf_down/F");
  t->Branch("csv_jet1_medium_sf_up", &csv_jet1_medium_sf_up, "csv_jet1_medium_sf_up/F");
  t->Branch("csv_jet1_nhf", &csv_jet1_nhf, "csv_jet1_nhf/F");
  t->Branch("csv_jet1_nlep", &csv_jet1_nlep, "csv_jet1_nlep/I");
  t->Branch("csv_jet1_phi", &csv_jet1_phi, "csv_jet1_phi/F");
  t->Branch("csv_jet1_pt", &csv_jet1_pt, "csv_jet1_pt/F");
  t->Branch("csv_jet1_pt_ratio", &csv_jet1_pt_ratio, "csv_jet1_pt_ratio/F");
  t->Branch("csv_jet1_pt_reg", &csv_jet1_pt_reg, "csv_jet1_pt_reg/F");
  t->Branch("csv_jet1_qgl", &csv_jet1_qgl, "csv_jet1_qgl/F");
  t->Branch("csv_jet1_tight_sf", &csv_jet1_tight_sf, "csv_jet1_tight_sf/F");
  t->Branch("csv_jet1_tight_sf_down", &csv_jet1_tight_sf_down, "csv_jet1_tight_sf_down/F");
  t->Branch("csv_jet1_tight_sf_up", &csv_jet1_tight_sf_up, "csv_jet1_tight_sf_up/F");
  t->Branch("csv_jet1_vtx_3Derr", &csv_jet1_vtx_3Derr, "csv_jet1_vtx_3Derr/F");
  t->Branch("csv_jet1_vtx_3Dval", &csv_jet1_vtx_3Dval, "csv_jet1_vtx_3Dval/F");
  t->Branch("csv_jet1_vtx_m", &csv_jet1_vtx_m, "csv_jet1_vtx_m/F");
  t->Branch("csv_jet1_vtx_ntrk", &csv_jet1_vtx_ntrk, "csv_jet1_vtx_ntrk/I");
  t->Branch("csv_jet1_vtx_pt", &csv_jet1_vtx_pt, "csv_jet1_vtx_pt/F");
  t->Branch("csv_jet2", &csv_jet2, "csv_jet2/O");
  t->Branch("csv_jet2_chf", &csv_jet2_chf, "csv_jet2_chf/F");
  t->Branch("csv_jet2_cmva", &csv_jet2_cmva, "csv_jet2_cmva/F");
  t->Branch("csv_jet2_csv", &csv_jet2_csv, "csv_jet2_csv/F");
  t->Branch("csv_jet2_deepcmvab", &csv_jet2_deepcmvab, "csv_jet2_deepcmvab/F");
  t->Branch("csv_jet2_deepcsvb", &csv_jet2_deepcsvb, "csv_jet2_deepcsvb/F");
  t->Branch("csv_jet2_emfrac", &csv_jet2_emfrac, "csv_jet2_emfrac/F");
  t->Branch("csv_jet2_eta", &csv_jet2_eta, "csv_jet2_eta/F");
  t->Branch("csv_jet2_gen", &csv_jet2_gen, "csv_jet2_gen/O");
  t->Branch("csv_jet2_gen_eta", &csv_jet2_gen_eta, "csv_jet2_gen_eta/F");
  t->Branch("csv_jet2_gen_m", &csv_jet2_gen_m, "csv_jet2_gen_m/F");
  t->Branch("csv_jet2_gen_num_b", &csv_jet2_gen_num_b, "csv_jet2_gen_num_b/I");
  t->Branch("csv_jet2_gen_parton_flav", &csv_jet2_gen_parton_flav, "csv_jet2_gen_parton_flav/I");
  t->Branch("csv_jet2_gen_pdgid", &csv_jet2_gen_pdgid, "csv_jet2_gen_pdgid/I");
  t->Branch("csv_jet2_gen_phi", &csv_jet2_gen_phi, "csv_jet2_gen_phi/F");
  t->Branch("csv_jet2_gen_pt", &csv_jet2_gen_pt, "csv_jet2_gen_pt/F");
  t->Branch("csv_jet2_leadlep_dr", &csv_jet2_leadlep_dr, "csv_jet2_leadlep_dr/F");
  t->Branch("csv_jet2_leadlep_pt", &csv_jet2_leadlep_pt, "csv_jet2_leadlep_pt/F");
  t->Branch("csv_jet2_leadlep_ptrel", &csv_jet2_leadlep_ptrel, "csv_jet2_leadlep_ptrel/F");
  t->Branch("csv_jet2_loose_sf", &csv_jet2_loose_sf, "csv_jet2_loose_sf/F");
  t->Branch("csv_jet2_loose_sf_down", &csv_jet2_loose_sf_down, "csv_jet2_loose_sf_down/F");
  t->Branch("csv_jet2_loose_sf_up", &csv_jet2_loose_sf_up, "csv_jet2_loose_sf_up/F");
  t->Branch("csv_jet2_m", &csv_jet2_m, "csv_jet2_m/F");
  t->Branch("csv_jet2_maxtrk", &csv_jet2_maxtrk, "csv_jet2_maxtrk/F");
  t->Branch("csv_jet2_medium_sf", &csv_jet2_medium_sf, "csv_jet2_medium_sf/F");
  t->Branch("csv_jet2_medium_sf_down", &csv_jet2_medium_sf_down, "csv_jet2_medium_sf_down/F");
  t->Branch("csv_jet2_medium_sf_up", &csv_jet2_medium_sf_up, "csv_jet2_medium_sf_up/F");
  t->Branch("csv_jet2_nhf", &csv_jet2_nhf, "csv_jet2_nhf/F");
  t->Branch("csv_jet2_nlep", &csv_jet2_nlep, "csv_jet2_nlep/I");
  t->Branch("csv_jet2_phi", &csv_jet2_phi, "csv_jet2_phi/F");
  t->Branch("csv_jet2_pt", &csv_jet2_pt, "csv_jet2_pt/F");
  t->Branch("csv_jet2_pt_ratio", &csv_jet2_pt_ratio, "csv_jet2_pt_ratio/F");
  t->Branch("csv_jet2_pt_reg", &csv_jet2_pt_reg, "csv_jet2_pt_reg/F");
  t->Branch("csv_jet2_qgl", &csv_jet2_qgl, "csv_jet2_qgl/F");
  t->Branch("csv_jet2_tight_sf", &csv_jet2_tight_sf, "csv_jet2_tight_sf/F");
  t->Branch("csv_jet2_tight_sf_down", &csv_jet2_tight_sf_down, "csv_jet2_tight_sf_down/F");
  t->Branch("csv_jet2_tight_sf_up", &csv_jet2_tight_sf_up, "csv_jet2_tight_sf_up/F");
  t->Branch("csv_jet2_vtx_3Derr", &csv_jet2_vtx_3Derr, "csv_jet2_vtx_3Derr/F");
  t->Branch("csv_jet2_vtx_3Dval", &csv_jet2_vtx_3Dval, "csv_jet2_vtx_3Dval/F");
  t->Branch("csv_jet2_vtx_m", &csv_jet2_vtx_m, "csv_jet2_vtx_m/F");
  t->Branch("csv_jet2_vtx_ntrk", &csv_jet2_vtx_ntrk, "csv_jet2_vtx_ntrk/I");
  t->Branch("csv_jet2_vtx_pt", &csv_jet2_vtx_pt, "csv_jet2_vtx_pt/F");
  t->Branch("dphi_met_trkmet", &dphi_met_trkmet, "dphi_met_trkmet/F");
  t->Branch("dphi_uh_cmva", &dphi_uh_cmva, "dphi_uh_cmva/F");
  t->Branch("dphi_uh_csv", &dphi_uh_csv, "dphi_uh_csv/F");
  t->Branch("ele1", &ele1, "ele1/O");
  t->Branch("ele1_1l_2m_3t", &ele1_1l_2m_3t, "ele1_1l_2m_3t/b");
  t->Branch("ele1_eta", &ele1_eta, "ele1_eta/F");
  t->Branch("ele1_phi", &ele1_phi, "ele1_phi/F");
  t->Branch("ele1_pt", &ele1_pt, "ele1_pt/F");
  t->Branch("ele1_q", &ele1_q, "ele1_q/B");
  t->Branch("ele2", &ele2, "ele2/O");
  t->Branch("ele2_1l_2m_3t", &ele2_1l_2m_3t, "ele2_1l_2m_3t/b");
  t->Branch("ele2_eta", &ele2_eta, "ele2_eta/F");
  t->Branch("ele2_phi", &ele2_phi, "ele2_phi/F");
  t->Branch("ele2_pt", &ele2_pt, "ele2_pt/F");
  t->Branch("ele2_q", &ele2_q, "ele2_q/B");
  t->Branch("event_num", &event_num, "event_num/l");
  t->Branch("gen_t", &gen_t, "gen_t/O");
  t->Branch("gen_t_eta", &gen_t_eta, "gen_t_eta/F");
  t->Branch("gen_t_m", &gen_t_m, "gen_t_m/F");
  t->Branch("gen_t_pdgid", &gen_t_pdgid, "gen_t_pdgid/I");
  t->Branch("gen_t_phi", &gen_t_phi, "gen_t_phi/F");
  t->Branch("gen_t_pt", &gen_t_pt, "gen_t_pt/F");
  t->Branch("gen_tbar", &gen_tbar, "gen_tbar/O");
  t->Branch("gen_tbar_eta", &gen_tbar_eta, "gen_tbar_eta/F");
  t->Branch("gen_tbar_m", &gen_tbar_m, "gen_tbar_m/F");
  t->Branch("gen_tbar_pdgid", &gen_tbar_pdgid, "gen_tbar_pdgid/I");
  t->Branch("gen_tbar_phi", &gen_tbar_phi, "gen_tbar_phi/F");
  t->Branch("gen_tbar_pt", &gen_tbar_pt, "gen_tbar_pt/F");
  t->Branch("genboson", &genboson, "genboson/O");
  t->Branch("genboson_eta", &genboson_eta, "genboson_eta/F");
  t->Branch("genboson_m", &genboson_m, "genboson_m/F");
  t->Branch("genboson_pdgid", &genboson_pdgid, "genboson_pdgid/I");
  t->Branch("genboson_phi", &genboson_phi, "genboson_phi/F");
  t->Branch("genboson_pt", &genboson_pt, "genboson_pt/F");
  t->Branch("jet1", &jet1, "jet1/O");
  t->Branch("jet1_chf", &jet1_chf, "jet1_chf/F");
  t->Branch("jet1_emfrac", &jet1_emfrac, "jet1_emfrac/F");
  t->Branch("jet1_eta", &jet1_eta, "jet1_eta/F");
  t->Branch("jet1_gen", &jet1_gen, "jet1_gen/O");
  t->Branch("jet1_gen_eta", &jet1_gen_eta, "jet1_gen_eta/F");
  t->Branch("jet1_gen_m", &jet1_gen_m, "jet1_gen_m/F");
  t->Branch("jet1_gen_num_b", &jet1_gen_num_b, "jet1_gen_num_b/I");
  t->Branch("jet1_gen_parton_flav", &jet1_gen_parton_flav, "jet1_gen_parton_flav/I");
  t->Branch("jet1_gen_pdgid", &jet1_gen_pdgid, "jet1_gen_pdgid/I");
  t->Branch("jet1_gen_phi", &jet1_gen_phi, "jet1_gen_phi/F");
  t->Branch("jet1_gen_pt", &jet1_gen_pt, "jet1_gen_pt/F");
  t->Branch("jet1_m", &jet1_m, "jet1_m/F");
  t->Branch("jet1_nhf", &jet1_nhf, "jet1_nhf/F");
  t->Branch("jet1_phi", &jet1_phi, "jet1_phi/F");
  t->Branch("jet1_pt", &jet1_pt, "jet1_pt/F");
  t->Branch("jet1_qgl", &jet1_qgl, "jet1_qgl/F");
  t->Branch("jet2", &jet2, "jet2/O");
  t->Branch("jet2_chf", &jet2_chf, "jet2_chf/F");
  t->Branch("jet2_emfrac", &jet2_emfrac, "jet2_emfrac/F");
  t->Branch("jet2_eta", &jet2_eta, "jet2_eta/F");
  t->Branch("jet2_gen", &jet2_gen, "jet2_gen/O");
  t->Branch("jet2_gen_eta", &jet2_gen_eta, "jet2_gen_eta/F");
  t->Branch("jet2_gen_m", &jet2_gen_m, "jet2_gen_m/F");
  t->Branch("jet2_gen_num_b", &jet2_gen_num_b, "jet2_gen_num_b/I");
  t->Branch("jet2_gen_parton_flav", &jet2_gen_parton_flav, "jet2_gen_parton_flav/I");
  t->Branch("jet2_gen_pdgid", &jet2_gen_pdgid, "jet2_gen_pdgid/I");
  t->Branch("jet2_gen_phi", &jet2_gen_phi, "jet2_gen_phi/F");
  t->Branch("jet2_gen_pt", &jet2_gen_pt, "jet2_gen_pt/F");
  t->Branch("jet2_m", &jet2_m, "jet2_m/F");
  t->Branch("jet2_nhf", &jet2_nhf, "jet2_nhf/F");
  t->Branch("jet2_phi", &jet2_phi, "jet2_phi/F");
  t->Branch("jet2_pt", &jet2_pt, "jet2_pt/F");
  t->Branch("jet2_qgl", &jet2_qgl, "jet2_qgl/F");
  t->Branch("jet3", &jet3, "jet3/O");
  t->Branch("jet3_chf", &jet3_chf, "jet3_chf/F");
  t->Branch("jet3_emfrac", &jet3_emfrac, "jet3_emfrac/F");
  t->Branch("jet3_eta", &jet3_eta, "jet3_eta/F");
  t->Branch("jet3_gen", &jet3_gen, "jet3_gen/O");
  t->Branch("jet3_gen_eta", &jet3_gen_eta, "jet3_gen_eta/F");
  t->Branch("jet3_gen_m", &jet3_gen_m, "jet3_gen_m/F");
  t->Branch("jet3_gen_num_b", &jet3_gen_num_b, "jet3_gen_num_b/I");
  t->Branch("jet3_gen_parton_flav", &jet3_gen_parton_flav, "jet3_gen_parton_flav/I");
  t->Branch("jet3_gen_pdgid", &jet3_gen_pdgid, "jet3_gen_pdgid/I");
  t->Branch("jet3_gen_phi", &jet3_gen_phi, "jet3_gen_phi/F");
  t->Branch("jet3_gen_pt", &jet3_gen_pt, "jet3_gen_pt/F");
  t->Branch("jet3_m", &jet3_m, "jet3_m/F");
  t->Branch("jet3_nhf", &jet3_nhf, "jet3_nhf/F");
  t->Branch("jet3_phi", &jet3_phi, "jet3_phi/F");
  t->Branch("jet3_pt", &jet3_pt, "jet3_pt/F");
  t->Branch("jet3_qgl", &jet3_qgl, "jet3_qgl/F");
  t->Branch("lumi_num", &lumi_num, "lumi_num/I");
  t->Branch("mc_weight", &mc_weight, "mc_weight/F");
  t->Branch("met", &met, "met/F");
  t->Branch("met_filter", &met_filter, "met_filter/O");
  t->Branch("met_trigger", &met_trigger, "met_trigger/O");
  t->Branch("metphi", &metphi, "metphi/F");
  t->Branch("min_dphi_metj_hard", &min_dphi_metj_hard, "min_dphi_metj_hard/F");
  t->Branch("min_dphi_metj_soft", &min_dphi_metj_soft, "min_dphi_metj_soft/F");
  t->Branch("muon1", &muon1, "muon1/O");
  t->Branch("muon1_1l_2m_3t", &muon1_1l_2m_3t, "muon1_1l_2m_3t/b");
  t->Branch("muon1_eta", &muon1_eta, "muon1_eta/F");
  t->Branch("muon1_phi", &muon1_phi, "muon1_phi/F");
  t->Branch("muon1_pt", &muon1_pt, "muon1_pt/F");
  t->Branch("muon1_q", &muon1_q, "muon1_q/B");
  t->Branch("muon2", &muon2, "muon2/O");
  t->Branch("muon2_1l_2m_3t", &muon2_1l_2m_3t, "muon2_1l_2m_3t/b");
  t->Branch("muon2_eta", &muon2_eta, "muon2_eta/F");
  t->Branch("muon2_phi", &muon2_phi, "muon2_phi/F");
  t->Branch("muon2_pt", &muon2_pt, "muon2_pt/F");
  t->Branch("muon2_q", &muon2_q, "muon2_q/B");
  t->Branch("n_alljet", &n_alljet, "n_alljet/I");
  t->Branch("n_bcmva_loose", &n_bcmva_loose, "n_bcmva_loose/I");
  t->Branch("n_bcmva_medium", &n_bcmva_medium, "n_bcmva_medium/I");
  t->Branch("n_bcmva_tight", &n_bcmva_tight, "n_bcmva_tight/I");
  t->Branch("n_bcsv_loose", &n_bcsv_loose, "n_bcsv_loose/I");
  t->Branch("n_bcsv_medium", &n_bcsv_medium, "n_bcsv_medium/I");
  t->Branch("n_bcsv_tight", &n_bcsv_tight, "n_bcsv_tight/I");
  t->Branch("n_hardjet", &n_hardjet, "n_hardjet/I");
  t->Branch("n_jet", &n_jet, "n_jet/I");
  t->Branch("n_lep_loose", &n_lep_loose, "n_lep_loose/I");
  t->Branch("n_lep_medium", &n_lep_medium, "n_lep_medium/I");
  t->Branch("n_lep_tight", &n_lep_tight, "n_lep_tight/I");
  t->Branch("n_pho_loose", &n_pho_loose, "n_pho_loose/I");
  t->Branch("n_tau_loose", &n_tau_loose, "n_tau_loose/I");
  t->Branch("npv", &npv, "npv/I");
  t->Branch("npv_true", &npv_true, "npv_true/I");
  t->Branch("recoil", &recoil, "recoil/F");
  t->Branch("recoilphi", &recoilphi, "recoilphi/F");
  t->Branch("run_num", &run_num, "run_num/I");
  t->Branch("trkmet", &trkmet, "trkmet/F");
  t->Branch("trkmetphi", &trkmetphi, "trkmetphi/F");
}

void hbbfile::reset(const panda::Event& event) {
  calomet = event.caloMet.pt;
  calometphi = event.caloMet.phi;
  cmva_hbb = false;
  cmva_hbb_eta = -5;
  cmva_hbb_m = -5;
  cmva_hbb_phi = -5;
  cmva_hbb_pt = -5;
  cmva_jet1 = false;
  cmva_jet1_chf = -5;
  cmva_jet1_cmva = -5;
  cmva_jet1_csv = -5;
  cmva_jet1_deepcmvab = -5;
  cmva_jet1_deepcsvb = -5;
  cmva_jet1_emfrac = -5;
  cmva_jet1_eta = -5;
  cmva_jet1_gen = false;
  cmva_jet1_gen_eta = -5;
  cmva_jet1_gen_m = -5;
  cmva_jet1_gen_num_b = 0;
  cmva_jet1_gen_parton_flav = 0;
  cmva_jet1_gen_pdgid = 0;
  cmva_jet1_gen_phi = -5;
  cmva_jet1_gen_pt = -5;
  cmva_jet1_leadlep_dr = 0;
  cmva_jet1_leadlep_pt = 0;
  cmva_jet1_leadlep_ptrel = 0;
  cmva_jet1_loose_sf = 1;
  cmva_jet1_loose_sf_down = 1;
  cmva_jet1_loose_sf_up = 1;
  cmva_jet1_m = -5;
  cmva_jet1_maxtrk = -5;
  cmva_jet1_medium_sf = 1;
  cmva_jet1_medium_sf_down = 1;
  cmva_jet1_medium_sf_up = 1;
  cmva_jet1_nhf = -5;
  cmva_jet1_nlep = 0;
  cmva_jet1_phi = -5;
  cmva_jet1_pt = -5;
  cmva_jet1_pt_ratio = 0;
  cmva_jet1_pt_reg = -5;
  cmva_jet1_qgl = -5;
  cmva_jet1_tight_sf = 1;
  cmva_jet1_tight_sf_down = 1;
  cmva_jet1_tight_sf_up = 1;
  cmva_jet1_vtx_3Derr = 0;
  cmva_jet1_vtx_3Dval = 0;
  cmva_jet1_vtx_m = 0;
  cmva_jet1_vtx_ntrk = 0;
  cmva_jet1_vtx_pt = 0;
  cmva_jet2 = false;
  cmva_jet2_chf = -5;
  cmva_jet2_cmva = -5;
  cmva_jet2_csv = -5;
  cmva_jet2_deepcmvab = -5;
  cmva_jet2_deepcsvb = -5;
  cmva_jet2_emfrac = -5;
  cmva_jet2_eta = -5;
  cmva_jet2_gen = false;
  cmva_jet2_gen_eta = -5;
  cmva_jet2_gen_m = -5;
  cmva_jet2_gen_num_b = 0;
  cmva_jet2_gen_parton_flav = 0;
  cmva_jet2_gen_pdgid = 0;
  cmva_jet2_gen_phi = -5;
  cmva_jet2_gen_pt = -5;
  cmva_jet2_leadlep_dr = 0;
  cmva_jet2_leadlep_pt = 0;
  cmva_jet2_leadlep_ptrel = 0;
  cmva_jet2_loose_sf = 1;
  cmva_jet2_loose_sf_down = 1;
  cmva_jet2_loose_sf_up = 1;
  cmva_jet2_m = -5;
  cmva_jet2_maxtrk = -5;
  cmva_jet2_medium_sf = 1;
  cmva_jet2_medium_sf_down = 1;
  cmva_jet2_medium_sf_up = 1;
  cmva_jet2_nhf = -5;
  cmva_jet2_nlep = 0;
  cmva_jet2_phi = -5;
  cmva_jet2_pt = -5;
  cmva_jet2_pt_ratio = 0;
  cmva_jet2_pt_reg = -5;
  cmva_jet2_qgl = -5;
  cmva_jet2_tight_sf = 1;
  cmva_jet2_tight_sf_down = 1;
  cmva_jet2_tight_sf_up = 1;
  cmva_jet2_vtx_3Derr = 0;
  cmva_jet2_vtx_3Dval = 0;
  cmva_jet2_vtx_m = 0;
  cmva_jet2_vtx_ntrk = 0;
  cmva_jet2_vtx_pt = 0;
  csv_hbb = false;
  csv_hbb_eta = -5;
  csv_hbb_m = -5;
  csv_hbb_phi = -5;
  csv_hbb_pt = -5;
  csv_jet1 = false;
  csv_jet1_chf = -5;
  csv_jet1_cmva = -5;
  csv_jet1_csv = -5;
  csv_jet1_deepcmvab = -5;
  csv_jet1_deepcsvb = -5;
  csv_jet1_emfrac = -5;
  csv_jet1_eta = -5;
  csv_jet1_gen = false;
  csv_jet1_gen_eta = -5;
  csv_jet1_gen_m = -5;
  csv_jet1_gen_num_b = 0;
  csv_jet1_gen_parton_flav = 0;
  csv_jet1_gen_pdgid = 0;
  csv_jet1_gen_phi = -5;
  csv_jet1_gen_pt = -5;
  csv_jet1_leadlep_dr = 0;
  csv_jet1_leadlep_pt = 0;
  csv_jet1_leadlep_ptrel = 0;
  csv_jet1_loose_sf = 1;
  csv_jet1_loose_sf_down = 1;
  csv_jet1_loose_sf_up = 1;
  csv_jet1_m = -5;
  csv_jet1_maxtrk = -5;
  csv_jet1_medium_sf = 1;
  csv_jet1_medium_sf_down = 1;
  csv_jet1_medium_sf_up = 1;
  csv_jet1_nhf = -5;
  csv_jet1_nlep = 0;
  csv_jet1_phi = -5;
  csv_jet1_pt = -5;
  csv_jet1_pt_ratio = 0;
  csv_jet1_pt_reg = -5;
  csv_jet1_qgl = -5;
  csv_jet1_tight_sf = 1;
  csv_jet1_tight_sf_down = 1;
  csv_jet1_tight_sf_up = 1;
  csv_jet1_vtx_3Derr = 0;
  csv_jet1_vtx_3Dval = 0;
  csv_jet1_vtx_m = 0;
  csv_jet1_vtx_ntrk = 0;
  csv_jet1_vtx_pt = 0;
  csv_jet2 = false;
  csv_jet2_chf = -5;
  csv_jet2_cmva = -5;
  csv_jet2_csv = -5;
  csv_jet2_deepcmvab = -5;
  csv_jet2_deepcsvb = -5;
  csv_jet2_emfrac = -5;
  csv_jet2_eta = -5;
  csv_jet2_gen = false;
  csv_jet2_gen_eta = -5;
  csv_jet2_gen_m = -5;
  csv_jet2_gen_num_b = 0;
  csv_jet2_gen_parton_flav = 0;
  csv_jet2_gen_pdgid = 0;
  csv_jet2_gen_phi = -5;
  csv_jet2_gen_pt = -5;
  csv_jet2_leadlep_dr = 0;
  csv_jet2_leadlep_pt = 0;
  csv_jet2_leadlep_ptrel = 0;
  csv_jet2_loose_sf = 1;
  csv_jet2_loose_sf_down = 1;
  csv_jet2_loose_sf_up = 1;
  csv_jet2_m = -5;
  csv_jet2_maxtrk = -5;
  csv_jet2_medium_sf = 1;
  csv_jet2_medium_sf_down = 1;
  csv_jet2_medium_sf_up = 1;
  csv_jet2_nhf = -5;
  csv_jet2_nlep = 0;
  csv_jet2_phi = -5;
  csv_jet2_pt = -5;
  csv_jet2_pt_ratio = 0;
  csv_jet2_pt_reg = -5;
  csv_jet2_qgl = -5;
  csv_jet2_tight_sf = 1;
  csv_jet2_tight_sf_down = 1;
  csv_jet2_tight_sf_up = 1;
  csv_jet2_vtx_3Derr = 0;
  csv_jet2_vtx_3Dval = 0;
  csv_jet2_vtx_m = 0;
  csv_jet2_vtx_ntrk = 0;
  csv_jet2_vtx_pt = 0;
  dphi_met_trkmet = -5;
  dphi_uh_cmva = -5;
  dphi_uh_csv = -5;
  ele1 = false;
  ele1_1l_2m_3t = 0;
  ele1_eta = -5;
  ele1_phi = -5;
  ele1_pt = -5;
  ele1_q = 0;
  ele2 = false;
  ele2_1l_2m_3t = 0;
  ele2_eta = -5;
  ele2_phi = -5;
  ele2_pt = -5;
  ele2_q = 0;
  event_num = event.eventNumber;
  gen_t = false;
  gen_t_eta = -5;
  gen_t_m = -5;
  gen_t_pdgid = 0;
  gen_t_phi = -5;
  gen_t_pt = -5;
  gen_tbar = false;
  gen_tbar_eta = -5;
  gen_tbar_m = -5;
  gen_tbar_pdgid = 0;
  gen_tbar_phi = -5;
  gen_tbar_pt = -5;
  genboson = false;
  genboson_eta = -5;
  genboson_m = -5;
  genboson_pdgid = 0;
  genboson_phi = -5;
  genboson_pt = -5;
  jet1 = false;
  jet1_chf = -5;
  jet1_emfrac = -5;
  jet1_eta = -5;
  jet1_gen = false;
  jet1_gen_eta = -5;
  jet1_gen_m = -5;
  jet1_gen_num_b = 0;
  jet1_gen_parton_flav = 0;
  jet1_gen_pdgid = 0;
  jet1_gen_phi = -5;
  jet1_gen_pt = -5;
  jet1_m = -5;
  jet1_nhf = -5;
  jet1_phi = -5;
  jet1_pt = -5;
  jet1_qgl = -5;
  jet2 = false;
  jet2_chf = -5;
  jet2_emfrac = -5;
  jet2_eta = -5;
  jet2_gen = false;
  jet2_gen_eta = -5;
  jet2_gen_m = -5;
  jet2_gen_num_b = 0;
  jet2_gen_parton_flav = 0;
  jet2_gen_pdgid = 0;
  jet2_gen_phi = -5;
  jet2_gen_pt = -5;
  jet2_m = -5;
  jet2_nhf = -5;
  jet2_phi = -5;
  jet2_pt = -5;
  jet2_qgl = -5;
  jet3 = false;
  jet3_chf = -5;
  jet3_emfrac = -5;
  jet3_eta = -5;
  jet3_gen = false;
  jet3_gen_eta = -5;
  jet3_gen_m = -5;
  jet3_gen_num_b = 0;
  jet3_gen_parton_flav = 0;
  jet3_gen_pdgid = 0;
  jet3_gen_phi = -5;
  jet3_gen_pt = -5;
  jet3_m = -5;
  jet3_nhf = -5;
  jet3_phi = -5;
  jet3_pt = -5;
  jet3_qgl = -5;
  lumi_num = event.lumiNumber;
  mc_weight = event.weight;
  met = event.pfMet.pt;
  met_filter = event.metFilters.pass();
  met_trigger = false;
  metphi = event.pfMet.phi;
  min_dphi_metj_hard = 4;
  min_dphi_metj_soft = 4;
  muon1 = false;
  muon1_1l_2m_3t = 0;
  muon1_eta = -5;
  muon1_phi = -5;
  muon1_pt = -5;
  muon1_q = 0;
  muon2 = false;
  muon2_1l_2m_3t = 0;
  muon2_eta = -5;
  muon2_phi = -5;
  muon2_pt = -5;
  muon2_q = 0;
  n_alljet = 0;
  n_bcmva_loose = 0;
  n_bcmva_medium = 0;
  n_bcmva_tight = 0;
  n_bcsv_loose = 0;
  n_bcsv_medium = 0;
  n_bcsv_tight = 0;
  n_hardjet = 0;
  n_jet = 0;
  n_lep_loose = 0;
  n_lep_medium = 0;
  n_lep_tight = 0;
  n_pho_loose = 0;
  n_tau_loose = 0;
  npv = event.npv;
  npv_true = event.npvTrue;
  recoil = -5;
  recoilphi = -5;
  run_num = event.runNumber;
  trkmet = event.trkMet.pt;
  trkmetphi = event.trkMet.phi;
}

void hbbfile::fill(const TVector2 recoilvec) {
  recoil = recoilvec.Mod();
  recoilphi = recoilvec.Phi();
  dphi_met_trkmet = deltaPhi(metphi, trkmetphi);
  dphi_uh_csv = deltaPhi(csv_hbb_phi, recoilphi);
  dphi_uh_cmva = deltaPhi(cmva_hbb_phi, recoilphi);
  t->Fill();
}
void hbbfile::set_lep(const lep base, const panda::Lepton& lep, const unsigned char flag) {
  auto& base_name = lep_names[static_cast<unsigned>(base)];
  set(base_name + "", static_cast<Bool_t>(true));
  set(base_name + "_1l_2m_3t", static_cast<UChar_t>(flag));
  set(base_name + "_q", static_cast<Char_t>(lep.charge));
  set(base_name + "_pt", static_cast<Float_t>(lep.pt()));
  set(base_name + "_eta", static_cast<Float_t>(lep.eta()));
  set(base_name + "_phi", static_cast<Float_t>(lep.phi()));
}

void hbbfile::set_bjet(const bjet base, const panda::Jet& jet, const float maxpt, const BCalReaders& readers, const BTagEntry::JetFlavor flav) {
  auto& base_name = bjet_names[static_cast<unsigned>(base)];
  set(base_name + "_maxtrk", static_cast<Float_t>(maxpt));
  set(base_name + "_csv", static_cast<Float_t>(jet.csv));
  set(base_name + "_cmva", static_cast<Float_t>(jet.cmva));
  set(base_name + "_deepcsvb", static_cast<Float_t>(jet.deepCSVb));
  set(base_name + "_deepcmvab", static_cast<Float_t>(jet.deepCMVAb));
  set(base_name + "_loose_sf", static_cast<Float_t>(readers.at(BTagEntry::OP_LOOSE).eval_auto_bounds("central", flav, jet.eta(), jet.pt())));
  set(base_name + "_loose_sf_up", static_cast<Float_t>(readers.at(BTagEntry::OP_LOOSE).eval_auto_bounds("up", flav, jet.eta(), jet.pt())));
  set(base_name + "_loose_sf_down", static_cast<Float_t>(readers.at(BTagEntry::OP_LOOSE).eval_auto_bounds("down", flav, jet.eta(), jet.pt())));
  set(base_name + "_medium_sf", static_cast<Float_t>(readers.at(BTagEntry::OP_MEDIUM).eval_auto_bounds("central", flav, jet.eta(), jet.pt())));
  set(base_name + "_medium_sf_up", static_cast<Float_t>(readers.at(BTagEntry::OP_MEDIUM).eval_auto_bounds("up", flav, jet.eta(), jet.pt())));
  set(base_name + "_medium_sf_down", static_cast<Float_t>(readers.at(BTagEntry::OP_MEDIUM).eval_auto_bounds("down", flav, jet.eta(), jet.pt())));
  set(base_name + "_tight_sf", static_cast<Float_t>(readers.at(BTagEntry::OP_TIGHT).eval_auto_bounds("central", flav, jet.eta(), jet.pt())));
  set(base_name + "_tight_sf_up", static_cast<Float_t>(readers.at(BTagEntry::OP_TIGHT).eval_auto_bounds("up", flav, jet.eta(), jet.pt())));
  set(base_name + "_tight_sf_down", static_cast<Float_t>(readers.at(BTagEntry::OP_TIGHT).eval_auto_bounds("down", flav, jet.eta(), jet.pt())));
}

void hbbfile::set_bvert(const bjet base, const panda::SecondaryVertex& vert) {
  auto& base_name = bjet_names[static_cast<unsigned>(base)];
  set(base_name + "_vtx_pt", static_cast<Float_t>(vert.pt()));
  set(base_name + "_vtx_m", static_cast<Float_t>(vert.m()));
  set(base_name + "_vtx_3Dval", static_cast<Float_t>(vert.vtx3DVal));
  set(base_name + "_vtx_3Derr", static_cast<Float_t>(vert.vtx3DeVal));
  set(base_name + "_vtx_ntrk", static_cast<Int_t>(vert.ntrk));
}

void hbbfile::set_bleps(const bjet base, const panda::Jet& jet, const int nlep, const panda::PFCand& lep) {
  auto& base_name = bjet_names[static_cast<unsigned>(base)];
  set(base_name + "_nlep", static_cast<Int_t>(nlep));
  set(base_name + "_leadlep_pt", static_cast<Float_t>(lep.pt()));
  set(base_name + "_leadlep_ptrel", static_cast<Float_t>(lep.p4().Perp(jet.p4().Vect())));
  set(base_name + "_leadlep_dr", static_cast<Float_t>(deltaR(lep.eta(), lep.phi(), jet.eta(), jet.phi())));
}

void hbbfile::set_pt_reg(const bjet base) {
  auto& base_name = bjet_names[static_cast<unsigned>(base)];
  set(base_name + "_pt_reg", static_cast<Float_t>((*(Float_t*)(addresses.at(base_name + "_pt_ratio"))) * (*(Float_t*)(addresses.at(base_name + "_pt")))));
}

void hbbfile::set_jet(const jet base, const panda::Jet& jet) {
  auto& base_name = jet_names[static_cast<unsigned>(base)];
  set(base_name + "", static_cast<Bool_t>(true));
  set(base_name + "_pt", static_cast<Float_t>(jet.pt()));
  set(base_name + "_eta", static_cast<Float_t>(jet.eta()));
  set(base_name + "_phi", static_cast<Float_t>(jet.phi()));
  set(base_name + "_m", static_cast<Float_t>(jet.m()));
  set(base_name + "_emfrac", static_cast<Float_t>(jet.cef + jet.nef));
  set(base_name + "_chf", static_cast<Float_t>(jet.chf));
  set(base_name + "_nhf", static_cast<Float_t>(jet.nhf));
  set(base_name + "_qgl", static_cast<Float_t>(jet.qgl));
}

void hbbfile::set_genjet(const jet base, const panda::GenJet& gen) {
  auto& base_name = jet_names[static_cast<unsigned>(base)];
  set(base_name + "_gen", static_cast<Bool_t>(true));
  set(base_name + "_gen_pt", static_cast<Float_t>(gen.pt()));
  set(base_name + "_gen_eta", static_cast<Float_t>(gen.eta()));
  set(base_name + "_gen_phi", static_cast<Float_t>(gen.phi()));
  set(base_name + "_gen_m", static_cast<Float_t>(gen.m()));
  set(base_name + "_gen_pdgid", static_cast<Int_t>(gen.pdgid));
  set(base_name + "_gen_parton_flav", static_cast<Int_t>(gen.partonFlavor));
  set(base_name + "_gen_num_b", static_cast<Int_t>(gen.numB));
}

void hbbfile::set_hbb(const hbb base, const TLorentzVector& vec) {
  auto& base_name = hbb_names[static_cast<unsigned>(base)];
  set(base_name + "", static_cast<Bool_t>(true));
  set(base_name + "_pt", static_cast<Float_t>(vec.Pt()));
  set(base_name + "_eta", static_cast<Float_t>(vec.Eta()));
  set(base_name + "_phi", static_cast<Float_t>(vec.Phi()));
  set(base_name + "_m", static_cast<Float_t>(vec.M()));
}

void hbbfile::set_gen(const gen base, const panda::GenParticle& gen) {
  auto& base_name = gen_names[static_cast<unsigned>(base)];
  set(base_name + "", static_cast<Bool_t>(true));
  set(base_name + "_pt", static_cast<Float_t>(gen.pt()));
  set(base_name + "_eta", static_cast<Float_t>(gen.eta()));
  set(base_name + "_phi", static_cast<Float_t>(gen.phi()));
  set(base_name + "_m", static_cast<Float_t>(gen.m()));
  set(base_name + "_pdgid", static_cast<Int_t>(gen.pdgid));
}


hbbfile::bjet to_bjet(hbbfile::jet e_cls) {
  switch (e_cls) {
  case hbbfile::jet::csv_jet1:
    return hbbfile::bjet::csv_jet1;
  case hbbfile::jet::csv_jet2:
    return hbbfile::bjet::csv_jet2;
  case hbbfile::jet::cmva_jet1:
    return hbbfile::bjet::cmva_jet1;
  case hbbfile::jet::cmva_jet2:
    return hbbfile::bjet::cmva_jet2;
  default:
    throw;
  }
}

hbbfile::jet to_jet(hbbfile::bjet e_cls) {
  switch (e_cls) {
  case hbbfile::bjet::csv_jet1:
    return hbbfile::jet::csv_jet1;
  case hbbfile::bjet::csv_jet2:
    return hbbfile::jet::csv_jet2;
  case hbbfile::bjet::cmva_jet1:
    return hbbfile::jet::cmva_jet1;
  case hbbfile::bjet::cmva_jet2:
    return hbbfile::jet::cmva_jet2;
  default:
    throw;
  }
}

#endif
