#ifndef CROMBIE_HBBFILE_H
#define CROMBIE_HBBFILE_H

#include <string>
#include <vector>
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "PlotTools/interface/KinematicFunctions.h"
#include "PandaTree/Objects/interface/Event.h"
#include "TLorentzVector.h"

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
  Float_t cmva_jet1_m;
  Float_t cmva_jet1_maxtrk;
  Float_t cmva_jet1_nhf;
  Int_t cmva_jet1_nlep;
  Float_t cmva_jet1_phi;
  Float_t cmva_jet1_pt;
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
  Float_t cmva_jet2_m;
  Float_t cmva_jet2_maxtrk;
  Float_t cmva_jet2_nhf;
  Int_t cmva_jet2_nlep;
  Float_t cmva_jet2_phi;
  Float_t cmva_jet2_pt;
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
  Float_t csv_jet1_m;
  Float_t csv_jet1_maxtrk;
  Float_t csv_jet1_nhf;
  Int_t csv_jet1_nlep;
  Float_t csv_jet1_phi;
  Float_t csv_jet1_pt;
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
  Float_t csv_jet2_m;
  Float_t csv_jet2_maxtrk;
  Float_t csv_jet2_nhf;
  Int_t csv_jet2_nlep;
  Float_t csv_jet2_phi;
  Float_t csv_jet2_pt;
  Float_t csv_jet2_vtx_3Derr;
  Float_t csv_jet2_vtx_3Dval;
  Float_t csv_jet2_vtx_m;
  Int_t csv_jet2_vtx_ntrk;
  Float_t csv_jet2_vtx_pt;
  Float_t ele1_eta;
  Float_t ele1_phi;
  Float_t ele1_pt;
  Float_t ele2_eta;
  Float_t ele2_phi;
  Float_t ele2_pt;
  Long_t event_num;
  Bool_t jet1;
  Float_t jet1_chf;
  Float_t jet1_cmva;
  Float_t jet1_csv;
  Float_t jet1_deepcmvab;
  Float_t jet1_deepcsvb;
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
  Bool_t jet2;
  Float_t jet2_chf;
  Float_t jet2_cmva;
  Float_t jet2_csv;
  Float_t jet2_deepcmvab;
  Float_t jet2_deepcsvb;
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
  Int_t lumi_num;
  Float_t mc_weight;
  Float_t met;
  Bool_t met_filter;
  Bool_t met_trigger;
  Float_t metphi;
  Float_t muon1_eta;
  Float_t muon1_phi;
  Float_t muon1_pt;
  Float_t muon2_eta;
  Float_t muon2_phi;
  Float_t muon2_pt;
  Int_t n_alljet;
  Int_t n_hardjet;
  Int_t n_jet;
  Int_t n_looselep;
  Int_t n_loosepho;
  Int_t n_loosetau;
  Int_t n_mediumlep;
  Int_t n_tightlep;
  Int_t npv;
  Int_t npv_true;
  Float_t recoil;
  Float_t recoilphi;
  Int_t run_num;
  Float_t trkmet;
  Float_t trkmetphi;

  void reset(panda::Event& event);
  void fill() { t->Fill(); }
  void write(TObject* obj) { f->WriteTObject(obj, obj->GetName()); }
  
  enum class lep_enum : int {
    muon1 = 0,
    muon2 = 1,
    ele1 = 2,
    ele2 = 3
  };
  const std::vector<std::string> lep_names = {
    "muon1",
    "muon2",
    "ele1",
    "ele2"
  };
  void set_lep(const lep_enum base, const panda::Lepton& lep);
  
  enum class bjet_enum : int {
    csv_jet1 = 0,
    csv_jet2 = 1,
    cmva_jet1 = 2,
    cmva_jet2 = 3
  };
  const std::vector<std::string> bjet_names = {
    "csv_jet1",
    "csv_jet2",
    "cmva_jet1",
    "cmva_jet2"
  };
  void set_bjet(const bjet_enum base, const panda::SecondaryVertex& vert);
  void set_bleps(const bjet_enum base, const panda::Jet& jet, const int nlep, const panda::PFCand& lep);
  void set_bmaxtrk(const bjet_enum base, const float maxpt);
  
  enum class jet_enum : int {
    csv_jet1 = 0,
    csv_jet2 = 1,
    cmva_jet1 = 2,
    cmva_jet2 = 3,
    jet1 = 4,
    jet2 = 5
  };
  const std::vector<std::string> jet_names = {
    "csv_jet1",
    "csv_jet2",
    "cmva_jet1",
    "cmva_jet2",
    "jet1",
    "jet2"
  };
  void set_jet(const jet_enum base, const panda::Jet& jet);
  void set_genjet(const jet_enum base, const panda::GenJet& gen);
  
  enum class hbb_enum : int {
    csv_hbb = 0,
    cmva_hbb = 1
  };
  const std::vector<std::string> hbb_names = {
    "csv_hbb",
    "cmva_hbb"
  };
  void set_hbb(const hbb_enum base, const TLorentzVector& vec);

 private:
  TFile* f;
  TTree* t;  

  template <typename T>
  void set(std::string name, T val) { *(T*)(t->GetBranch(name.data())->GetAddress()) = val; }

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
  t->Branch("cmva_jet1_m", &cmva_jet1_m, "cmva_jet1_m/F");
  t->Branch("cmva_jet1_maxtrk", &cmva_jet1_maxtrk, "cmva_jet1_maxtrk/F");
  t->Branch("cmva_jet1_nhf", &cmva_jet1_nhf, "cmva_jet1_nhf/F");
  t->Branch("cmva_jet1_nlep", &cmva_jet1_nlep, "cmva_jet1_nlep/I");
  t->Branch("cmva_jet1_phi", &cmva_jet1_phi, "cmva_jet1_phi/F");
  t->Branch("cmva_jet1_pt", &cmva_jet1_pt, "cmva_jet1_pt/F");
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
  t->Branch("cmva_jet2_m", &cmva_jet2_m, "cmva_jet2_m/F");
  t->Branch("cmva_jet2_maxtrk", &cmva_jet2_maxtrk, "cmva_jet2_maxtrk/F");
  t->Branch("cmva_jet2_nhf", &cmva_jet2_nhf, "cmva_jet2_nhf/F");
  t->Branch("cmva_jet2_nlep", &cmva_jet2_nlep, "cmva_jet2_nlep/I");
  t->Branch("cmva_jet2_phi", &cmva_jet2_phi, "cmva_jet2_phi/F");
  t->Branch("cmva_jet2_pt", &cmva_jet2_pt, "cmva_jet2_pt/F");
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
  t->Branch("csv_jet1_m", &csv_jet1_m, "csv_jet1_m/F");
  t->Branch("csv_jet1_maxtrk", &csv_jet1_maxtrk, "csv_jet1_maxtrk/F");
  t->Branch("csv_jet1_nhf", &csv_jet1_nhf, "csv_jet1_nhf/F");
  t->Branch("csv_jet1_nlep", &csv_jet1_nlep, "csv_jet1_nlep/I");
  t->Branch("csv_jet1_phi", &csv_jet1_phi, "csv_jet1_phi/F");
  t->Branch("csv_jet1_pt", &csv_jet1_pt, "csv_jet1_pt/F");
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
  t->Branch("csv_jet2_m", &csv_jet2_m, "csv_jet2_m/F");
  t->Branch("csv_jet2_maxtrk", &csv_jet2_maxtrk, "csv_jet2_maxtrk/F");
  t->Branch("csv_jet2_nhf", &csv_jet2_nhf, "csv_jet2_nhf/F");
  t->Branch("csv_jet2_nlep", &csv_jet2_nlep, "csv_jet2_nlep/I");
  t->Branch("csv_jet2_phi", &csv_jet2_phi, "csv_jet2_phi/F");
  t->Branch("csv_jet2_pt", &csv_jet2_pt, "csv_jet2_pt/F");
  t->Branch("csv_jet2_vtx_3Derr", &csv_jet2_vtx_3Derr, "csv_jet2_vtx_3Derr/F");
  t->Branch("csv_jet2_vtx_3Dval", &csv_jet2_vtx_3Dval, "csv_jet2_vtx_3Dval/F");
  t->Branch("csv_jet2_vtx_m", &csv_jet2_vtx_m, "csv_jet2_vtx_m/F");
  t->Branch("csv_jet2_vtx_ntrk", &csv_jet2_vtx_ntrk, "csv_jet2_vtx_ntrk/I");
  t->Branch("csv_jet2_vtx_pt", &csv_jet2_vtx_pt, "csv_jet2_vtx_pt/F");
  t->Branch("ele1_eta", &ele1_eta, "ele1_eta/F");
  t->Branch("ele1_phi", &ele1_phi, "ele1_phi/F");
  t->Branch("ele1_pt", &ele1_pt, "ele1_pt/F");
  t->Branch("ele2_eta", &ele2_eta, "ele2_eta/F");
  t->Branch("ele2_phi", &ele2_phi, "ele2_phi/F");
  t->Branch("ele2_pt", &ele2_pt, "ele2_pt/F");
  t->Branch("event_num", &event_num, "event_num/l");
  t->Branch("jet1", &jet1, "jet1/O");
  t->Branch("jet1_chf", &jet1_chf, "jet1_chf/F");
  t->Branch("jet1_cmva", &jet1_cmva, "jet1_cmva/F");
  t->Branch("jet1_csv", &jet1_csv, "jet1_csv/F");
  t->Branch("jet1_deepcmvab", &jet1_deepcmvab, "jet1_deepcmvab/F");
  t->Branch("jet1_deepcsvb", &jet1_deepcsvb, "jet1_deepcsvb/F");
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
  t->Branch("jet2", &jet2, "jet2/O");
  t->Branch("jet2_chf", &jet2_chf, "jet2_chf/F");
  t->Branch("jet2_cmva", &jet2_cmva, "jet2_cmva/F");
  t->Branch("jet2_csv", &jet2_csv, "jet2_csv/F");
  t->Branch("jet2_deepcmvab", &jet2_deepcmvab, "jet2_deepcmvab/F");
  t->Branch("jet2_deepcsvb", &jet2_deepcsvb, "jet2_deepcsvb/F");
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
  t->Branch("lumi_num", &lumi_num, "lumi_num/I");
  t->Branch("mc_weight", &mc_weight, "mc_weight/F");
  t->Branch("met", &met, "met/F");
  t->Branch("met_filter", &met_filter, "met_filter/O");
  t->Branch("met_trigger", &met_trigger, "met_trigger/O");
  t->Branch("metphi", &metphi, "metphi/F");
  t->Branch("muon1_eta", &muon1_eta, "muon1_eta/F");
  t->Branch("muon1_phi", &muon1_phi, "muon1_phi/F");
  t->Branch("muon1_pt", &muon1_pt, "muon1_pt/F");
  t->Branch("muon2_eta", &muon2_eta, "muon2_eta/F");
  t->Branch("muon2_phi", &muon2_phi, "muon2_phi/F");
  t->Branch("muon2_pt", &muon2_pt, "muon2_pt/F");
  t->Branch("n_alljet", &n_alljet, "n_alljet/I");
  t->Branch("n_hardjet", &n_hardjet, "n_hardjet/I");
  t->Branch("n_jet", &n_jet, "n_jet/I");
  t->Branch("n_looselep", &n_looselep, "n_looselep/I");
  t->Branch("n_loosepho", &n_loosepho, "n_loosepho/I");
  t->Branch("n_loosetau", &n_loosetau, "n_loosetau/I");
  t->Branch("n_mediumlep", &n_mediumlep, "n_mediumlep/I");
  t->Branch("n_tightlep", &n_tightlep, "n_tightlep/I");
  t->Branch("npv", &npv, "npv/I");
  t->Branch("npv_true", &npv_true, "npv_true/I");
  t->Branch("recoil", &recoil, "recoil/F");
  t->Branch("recoilphi", &recoilphi, "recoilphi/F");
  t->Branch("run_num", &run_num, "run_num/I");
  t->Branch("trkmet", &trkmet, "trkmet/F");
  t->Branch("trkmetphi", &trkmetphi, "trkmetphi/F");
}


void hbbfile::reset(panda::Event& event) {
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
  cmva_jet1_m = -5;
  cmva_jet1_maxtrk = 0;
  cmva_jet1_nhf = -5;
  cmva_jet1_nlep = 0;
  cmva_jet1_phi = -5;
  cmva_jet1_pt = -5;
  cmva_jet1_vtx_3Derr = -5;
  cmva_jet1_vtx_3Dval = -5;
  cmva_jet1_vtx_m = -5;
  cmva_jet1_vtx_ntrk = -5;
  cmva_jet1_vtx_pt = -5;
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
  cmva_jet2_m = -5;
  cmva_jet2_maxtrk = 0;
  cmva_jet2_nhf = -5;
  cmva_jet2_nlep = 0;
  cmva_jet2_phi = -5;
  cmva_jet2_pt = -5;
  cmva_jet2_vtx_3Derr = -5;
  cmva_jet2_vtx_3Dval = -5;
  cmva_jet2_vtx_m = -5;
  cmva_jet2_vtx_ntrk = -5;
  cmva_jet2_vtx_pt = -5;
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
  csv_jet1_m = -5;
  csv_jet1_maxtrk = 0;
  csv_jet1_nhf = -5;
  csv_jet1_nlep = 0;
  csv_jet1_phi = -5;
  csv_jet1_pt = -5;
  csv_jet1_vtx_3Derr = -5;
  csv_jet1_vtx_3Dval = -5;
  csv_jet1_vtx_m = -5;
  csv_jet1_vtx_ntrk = -5;
  csv_jet1_vtx_pt = -5;
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
  csv_jet2_m = -5;
  csv_jet2_maxtrk = 0;
  csv_jet2_nhf = -5;
  csv_jet2_nlep = 0;
  csv_jet2_phi = -5;
  csv_jet2_pt = -5;
  csv_jet2_vtx_3Derr = -5;
  csv_jet2_vtx_3Dval = -5;
  csv_jet2_vtx_m = -5;
  csv_jet2_vtx_ntrk = -5;
  csv_jet2_vtx_pt = -5;
  ele1_eta = -5;
  ele1_phi = -5;
  ele1_pt = -5;
  ele2_eta = -5;
  ele2_phi = -5;
  ele2_pt = -5;
  event_num = event.eventNumber;
  jet1 = false;
  jet1_chf = -5;
  jet1_cmva = -5;
  jet1_csv = -5;
  jet1_deepcmvab = -5;
  jet1_deepcsvb = -5;
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
  jet2 = false;
  jet2_chf = -5;
  jet2_cmva = -5;
  jet2_csv = -5;
  jet2_deepcmvab = -5;
  jet2_deepcsvb = -5;
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
  lumi_num = event.lumiNumber;
  mc_weight = event.weight;
  met = event.pfMet.pt;
  met_filter = event.metFilters.pass();
  met_trigger = false;
  metphi = event.pfMet.phi;
  muon1_eta = -5;
  muon1_phi = -5;
  muon1_pt = -5;
  muon2_eta = -5;
  muon2_phi = -5;
  muon2_pt = -5;
  n_alljet = 0;
  n_hardjet = 0;
  n_jet = 0;
  n_looselep = 0;
  n_loosepho = 0;
  n_loosetau = 0;
  n_mediumlep = 0;
  n_tightlep = 0;
  npv = event.npv;
  npv_true = event.npvTrue;
  recoil = -5;
  recoilphi = -5;
  run_num = event.runNumber;
  trkmet = event.trkMet.pt;
  trkmetphi = event.trkMet.phi;
}

void hbbfile::set_lep(const lep_enum base, const panda::Lepton& lep) {
  set(lep_names[static_cast<int>(base)] + "_pt", static_cast<Float_t>(lep.pt()));
  set(lep_names[static_cast<int>(base)] + "_eta", static_cast<Float_t>(lep.eta()));
  set(lep_names[static_cast<int>(base)] + "_phi", static_cast<Float_t>(lep.phi()));
}

void hbbfile::set_bjet(const bjet_enum base, const panda::SecondaryVertex& vert) {
  set(bjet_names[static_cast<int>(base)] + "_vtx_pt", static_cast<Float_t>(vert.pt()));
  set(bjet_names[static_cast<int>(base)] + "_vtx_m", static_cast<Float_t>(vert.m()));
  set(bjet_names[static_cast<int>(base)] + "_vtx_3Dval", static_cast<Float_t>(vert.vtx3DVal));
  set(bjet_names[static_cast<int>(base)] + "_vtx_3Derr", static_cast<Float_t>(vert.vtx3DeVal));
  set(bjet_names[static_cast<int>(base)] + "_vtx_ntrk", static_cast<Int_t>(vert.ntrk));
}

void hbbfile::set_bleps(const bjet_enum base, const panda::Jet& jet, const int nlep, const panda::PFCand& lep) {
  set(bjet_names[static_cast<int>(base)] + "_nlep", static_cast<Int_t>(nlep));
  set(bjet_names[static_cast<int>(base)] + "_leadlep_pt", static_cast<Float_t>(lep.pt()));
  set(bjet_names[static_cast<int>(base)] + "_leadlep_ptrel", static_cast<Float_t>(lep.p4().Perp(jet.p4().Vect())));
  set(bjet_names[static_cast<int>(base)] + "_leadlep_dr", static_cast<Float_t>(deltaR(lep.eta(), lep.phi(), jet.eta(), jet.phi())));
}

void hbbfile::set_bmaxtrk(const bjet_enum base, const float maxpt) {
  set(bjet_names[static_cast<int>(base)] + "_maxtrk", static_cast<Float_t>(maxpt));
}

void hbbfile::set_jet(const jet_enum base, const panda::Jet& jet) {
  set(jet_names[static_cast<int>(base)] + "", static_cast<Bool_t>(true));
  set(jet_names[static_cast<int>(base)] + "_pt", static_cast<Float_t>(jet.pt()));
  set(jet_names[static_cast<int>(base)] + "_eta", static_cast<Float_t>(jet.eta()));
  set(jet_names[static_cast<int>(base)] + "_phi", static_cast<Float_t>(jet.phi()));
  set(jet_names[static_cast<int>(base)] + "_m", static_cast<Float_t>(jet.m()));
  set(jet_names[static_cast<int>(base)] + "_emfrac", static_cast<Float_t>(jet.cef + jet.nef));
  set(jet_names[static_cast<int>(base)] + "_chf", static_cast<Float_t>(jet.chf));
  set(jet_names[static_cast<int>(base)] + "_nhf", static_cast<Float_t>(jet.nhf));
  set(jet_names[static_cast<int>(base)] + "_csv", static_cast<Float_t>(jet.csv));
  set(jet_names[static_cast<int>(base)] + "_cmva", static_cast<Float_t>(jet.cmva));
  set(jet_names[static_cast<int>(base)] + "_deepcsvb", static_cast<Float_t>(jet.deepCSVb));
  set(jet_names[static_cast<int>(base)] + "_deepcmvab", static_cast<Float_t>(jet.deepCMVAb));
}

void hbbfile::set_genjet(const jet_enum base, const panda::GenJet& gen) {
  set(jet_names[static_cast<int>(base)] + "_gen", static_cast<Bool_t>(true));
  set(jet_names[static_cast<int>(base)] + "_gen_pt", static_cast<Float_t>(gen.pt()));
  set(jet_names[static_cast<int>(base)] + "_gen_eta", static_cast<Float_t>(gen.eta()));
  set(jet_names[static_cast<int>(base)] + "_gen_phi", static_cast<Float_t>(gen.phi()));
  set(jet_names[static_cast<int>(base)] + "_gen_m", static_cast<Float_t>(gen.m()));
  set(jet_names[static_cast<int>(base)] + "_gen_pdgid", static_cast<Int_t>(gen.pdgid));
  set(jet_names[static_cast<int>(base)] + "_gen_parton_flav", static_cast<Int_t>(gen.partonFlavor));
  set(jet_names[static_cast<int>(base)] + "_gen_num_b", static_cast<Int_t>(gen.numB));
}

void hbbfile::set_hbb(const hbb_enum base, const TLorentzVector& vec) {
  set(hbb_names[static_cast<int>(base)] + "", static_cast<Bool_t>(true));
  set(hbb_names[static_cast<int>(base)] + "_pt", static_cast<Float_t>(vec.Pt()));
  set(hbb_names[static_cast<int>(base)] + "_eta", static_cast<Float_t>(vec.Eta()));
  set(hbb_names[static_cast<int>(base)] + "_phi", static_cast<Float_t>(vec.Phi()));
  set(hbb_names[static_cast<int>(base)] + "_m", static_cast<Float_t>(vec.M()));
}

#endif
