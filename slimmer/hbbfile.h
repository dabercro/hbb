#ifndef CROMBIE_HBBFILE_H
#define CROMBIE_HBBFILE_H

#include <string>
#include <vector>
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "PlotTools/interface/KinematicFunctions.h"
#include "PandaTree/Objects/interface/Event.h"
#include <algorithm>
#include "TLorentzVector.h"

class hbbfile {

 public:
  hbbfile(const char* outfile_name, const char* name = "events");
  ~hbbfile() { write(t); f->Close(); }

  float calomet;
  float calometphi;
  float cmva_hbb_eta;
  float cmva_hbb_m;
  float cmva_hbb_phi;
  float cmva_hbb_pt;
  bool cmva_jet1;
  float cmva_jet1_chf;
  float cmva_jet1_cmva;
  float cmva_jet1_csv;
  float cmva_jet1_emfrac;
  float cmva_jet1_eta;
  float cmva_jet1_m;
  float cmva_jet1_maxtrk;
  float cmva_jet1_nhf;
  int cmva_jet1_nlep;
  float cmva_jet1_phi;
  float cmva_jet1_pt;
  float cmva_jet1_vtx_3Derr;
  float cmva_jet1_vtx_3Dval;
  float cmva_jet1_vtx_m;
  float cmva_jet1_vtx_ntrk;
  float cmva_jet1_vtx_pt;
  bool cmva_jet2;
  float cmva_jet2_chf;
  float cmva_jet2_cmva;
  float cmva_jet2_csv;
  float cmva_jet2_emfrac;
  float cmva_jet2_eta;
  float cmva_jet2_m;
  float cmva_jet2_maxtrk;
  float cmva_jet2_nhf;
  int cmva_jet2_nlep;
  float cmva_jet2_phi;
  float cmva_jet2_pt;
  float cmva_jet2_vtx_3Derr;
  float cmva_jet2_vtx_3Dval;
  float cmva_jet2_vtx_m;
  float cmva_jet2_vtx_ntrk;
  float cmva_jet2_vtx_pt;
  float csv_hbb_eta;
  float csv_hbb_m;
  float csv_hbb_phi;
  float csv_hbb_pt;
  bool csv_jet1;
  float csv_jet1_chf;
  float csv_jet1_cmva;
  float csv_jet1_csv;
  float csv_jet1_emfrac;
  float csv_jet1_eta;
  float csv_jet1_m;
  float csv_jet1_maxtrk;
  float csv_jet1_nhf;
  int csv_jet1_nlep;
  float csv_jet1_phi;
  float csv_jet1_pt;
  float csv_jet1_vtx_3Derr;
  float csv_jet1_vtx_3Dval;
  float csv_jet1_vtx_m;
  float csv_jet1_vtx_ntrk;
  float csv_jet1_vtx_pt;
  bool csv_jet2;
  float csv_jet2_chf;
  float csv_jet2_cmva;
  float csv_jet2_csv;
  float csv_jet2_emfrac;
  float csv_jet2_eta;
  float csv_jet2_m;
  float csv_jet2_maxtrk;
  float csv_jet2_nhf;
  int csv_jet2_nlep;
  float csv_jet2_phi;
  float csv_jet2_pt;
  float csv_jet2_vtx_3Derr;
  float csv_jet2_vtx_3Dval;
  float csv_jet2_vtx_m;
  float csv_jet2_vtx_ntrk;
  float csv_jet2_vtx_pt;
  long event_num;
  float jet1_chf;
  float jet1_cmva;
  float jet1_csv;
  float jet1_emfrac;
  float jet1_eta;
  float jet1_m;
  float jet1_nhf;
  float jet1_phi;
  float jet1_pt;
  float jet2_chf;
  float jet2_cmva;
  float jet2_csv;
  float jet2_emfrac;
  float jet2_eta;
  float jet2_m;
  float jet2_nhf;
  float jet2_phi;
  float jet2_pt;
  int lumi_num;
  float mc_weight;
  float met;
  bool met_trigger;
  bool metfilter;
  float metphi;
  int n_alljet;
  int n_hardjet;
  int n_jet;
  int n_looselep;
  int n_loosepho;
  int n_loosetau;
  int n_mediumlep;
  int n_tightlep;
  int npv;
  int npv_true;
  float recoil;
  float recoilphi;
  int run_num;
  float trkmet;
  float trkmetphi;

  void reset(panda::Event& event);
  void fill() { t->Fill(); }
  void write(TObject* obj) { f->WriteTObject(obj, obj->GetName()); }

  
  enum class set_b_jet_enum : int {
    csv_jet1 = 0,
    csv_jet2 = 1,
    cmva_jet1 = 2,
    cmva_jet2 = 3
  };
  const std::vector<std::string> set_b_jet_names = {
    "csv_jet1",
    "csv_jet2",
    "cmva_jet1",
    "cmva_jet2"
  };
  void set_b_jet(const set_b_jet_enum base, const panda::Ref<panda::SecondaryVertex>& vert);
  
  void set_b_leps(const set_b_jet_enum base, const panda::Jet& jet, const int nlep, const panda::Ref<panda::PFCand>& pf);
  
  void set_b_max_trk(const set_b_jet_enum base, const float maxpt);
  
  enum class set_jet_enum : int {
    csv_jet1 = 0,
    csv_jet2 = 1,
    cmva_jet1 = 2,
    cmva_jet2 = 3,
    jet1 = 4,
    jet2 = 5
  };
  const std::vector<std::string> set_jet_names = {
    "csv_jet1",
    "csv_jet2",
    "cmva_jet1",
    "cmva_jet2",
    "jet1",
    "jet2"
  };
  void set_jet(const set_jet_enum base, const panda::Jet& jet);
  
  enum class set_hbb_enum : int {
    csv_hbb = 0,
    cmva_hbb = 1
  };
  const std::vector<std::string> set_hbb_names = {
    "csv_hbb",
    "cmva_hbb"
  };
  void set_hbb(const set_hbb_enum base, const TLorentzVector& vec);

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
  t->Branch("cmva_hbb_eta", &cmva_hbb_eta, "cmva_hbb_eta/F");
  t->Branch("cmva_hbb_m", &cmva_hbb_m, "cmva_hbb_m/F");
  t->Branch("cmva_hbb_phi", &cmva_hbb_phi, "cmva_hbb_phi/F");
  t->Branch("cmva_hbb_pt", &cmva_hbb_pt, "cmva_hbb_pt/F");
  t->Branch("cmva_jet1", &cmva_jet1, "cmva_jet1/O");
  t->Branch("cmva_jet1_chf", &cmva_jet1_chf, "cmva_jet1_chf/F");
  t->Branch("cmva_jet1_cmva", &cmva_jet1_cmva, "cmva_jet1_cmva/F");
  t->Branch("cmva_jet1_csv", &cmva_jet1_csv, "cmva_jet1_csv/F");
  t->Branch("cmva_jet1_emfrac", &cmva_jet1_emfrac, "cmva_jet1_emfrac/F");
  t->Branch("cmva_jet1_eta", &cmva_jet1_eta, "cmva_jet1_eta/F");
  t->Branch("cmva_jet1_m", &cmva_jet1_m, "cmva_jet1_m/F");
  t->Branch("cmva_jet1_maxtrk", &cmva_jet1_maxtrk, "cmva_jet1_maxtrk/F");
  t->Branch("cmva_jet1_nhf", &cmva_jet1_nhf, "cmva_jet1_nhf/F");
  t->Branch("cmva_jet1_nlep", &cmva_jet1_nlep, "cmva_jet1_nlep/I");
  t->Branch("cmva_jet1_phi", &cmva_jet1_phi, "cmva_jet1_phi/F");
  t->Branch("cmva_jet1_pt", &cmva_jet1_pt, "cmva_jet1_pt/F");
  t->Branch("cmva_jet1_vtx_3Derr", &cmva_jet1_vtx_3Derr, "cmva_jet1_vtx_3Derr/F");
  t->Branch("cmva_jet1_vtx_3Dval", &cmva_jet1_vtx_3Dval, "cmva_jet1_vtx_3Dval/F");
  t->Branch("cmva_jet1_vtx_m", &cmva_jet1_vtx_m, "cmva_jet1_vtx_m/F");
  t->Branch("cmva_jet1_vtx_ntrk", &cmva_jet1_vtx_ntrk, "cmva_jet1_vtx_ntrk/F");
  t->Branch("cmva_jet1_vtx_pt", &cmva_jet1_vtx_pt, "cmva_jet1_vtx_pt/F");
  t->Branch("cmva_jet2", &cmva_jet2, "cmva_jet2/O");
  t->Branch("cmva_jet2_chf", &cmva_jet2_chf, "cmva_jet2_chf/F");
  t->Branch("cmva_jet2_cmva", &cmva_jet2_cmva, "cmva_jet2_cmva/F");
  t->Branch("cmva_jet2_csv", &cmva_jet2_csv, "cmva_jet2_csv/F");
  t->Branch("cmva_jet2_emfrac", &cmva_jet2_emfrac, "cmva_jet2_emfrac/F");
  t->Branch("cmva_jet2_eta", &cmva_jet2_eta, "cmva_jet2_eta/F");
  t->Branch("cmva_jet2_m", &cmva_jet2_m, "cmva_jet2_m/F");
  t->Branch("cmva_jet2_maxtrk", &cmva_jet2_maxtrk, "cmva_jet2_maxtrk/F");
  t->Branch("cmva_jet2_nhf", &cmva_jet2_nhf, "cmva_jet2_nhf/F");
  t->Branch("cmva_jet2_nlep", &cmva_jet2_nlep, "cmva_jet2_nlep/I");
  t->Branch("cmva_jet2_phi", &cmva_jet2_phi, "cmva_jet2_phi/F");
  t->Branch("cmva_jet2_pt", &cmva_jet2_pt, "cmva_jet2_pt/F");
  t->Branch("cmva_jet2_vtx_3Derr", &cmva_jet2_vtx_3Derr, "cmva_jet2_vtx_3Derr/F");
  t->Branch("cmva_jet2_vtx_3Dval", &cmva_jet2_vtx_3Dval, "cmva_jet2_vtx_3Dval/F");
  t->Branch("cmva_jet2_vtx_m", &cmva_jet2_vtx_m, "cmva_jet2_vtx_m/F");
  t->Branch("cmva_jet2_vtx_ntrk", &cmva_jet2_vtx_ntrk, "cmva_jet2_vtx_ntrk/F");
  t->Branch("cmva_jet2_vtx_pt", &cmva_jet2_vtx_pt, "cmva_jet2_vtx_pt/F");
  t->Branch("csv_hbb_eta", &csv_hbb_eta, "csv_hbb_eta/F");
  t->Branch("csv_hbb_m", &csv_hbb_m, "csv_hbb_m/F");
  t->Branch("csv_hbb_phi", &csv_hbb_phi, "csv_hbb_phi/F");
  t->Branch("csv_hbb_pt", &csv_hbb_pt, "csv_hbb_pt/F");
  t->Branch("csv_jet1", &csv_jet1, "csv_jet1/O");
  t->Branch("csv_jet1_chf", &csv_jet1_chf, "csv_jet1_chf/F");
  t->Branch("csv_jet1_cmva", &csv_jet1_cmva, "csv_jet1_cmva/F");
  t->Branch("csv_jet1_csv", &csv_jet1_csv, "csv_jet1_csv/F");
  t->Branch("csv_jet1_emfrac", &csv_jet1_emfrac, "csv_jet1_emfrac/F");
  t->Branch("csv_jet1_eta", &csv_jet1_eta, "csv_jet1_eta/F");
  t->Branch("csv_jet1_m", &csv_jet1_m, "csv_jet1_m/F");
  t->Branch("csv_jet1_maxtrk", &csv_jet1_maxtrk, "csv_jet1_maxtrk/F");
  t->Branch("csv_jet1_nhf", &csv_jet1_nhf, "csv_jet1_nhf/F");
  t->Branch("csv_jet1_nlep", &csv_jet1_nlep, "csv_jet1_nlep/I");
  t->Branch("csv_jet1_phi", &csv_jet1_phi, "csv_jet1_phi/F");
  t->Branch("csv_jet1_pt", &csv_jet1_pt, "csv_jet1_pt/F");
  t->Branch("csv_jet1_vtx_3Derr", &csv_jet1_vtx_3Derr, "csv_jet1_vtx_3Derr/F");
  t->Branch("csv_jet1_vtx_3Dval", &csv_jet1_vtx_3Dval, "csv_jet1_vtx_3Dval/F");
  t->Branch("csv_jet1_vtx_m", &csv_jet1_vtx_m, "csv_jet1_vtx_m/F");
  t->Branch("csv_jet1_vtx_ntrk", &csv_jet1_vtx_ntrk, "csv_jet1_vtx_ntrk/F");
  t->Branch("csv_jet1_vtx_pt", &csv_jet1_vtx_pt, "csv_jet1_vtx_pt/F");
  t->Branch("csv_jet2", &csv_jet2, "csv_jet2/O");
  t->Branch("csv_jet2_chf", &csv_jet2_chf, "csv_jet2_chf/F");
  t->Branch("csv_jet2_cmva", &csv_jet2_cmva, "csv_jet2_cmva/F");
  t->Branch("csv_jet2_csv", &csv_jet2_csv, "csv_jet2_csv/F");
  t->Branch("csv_jet2_emfrac", &csv_jet2_emfrac, "csv_jet2_emfrac/F");
  t->Branch("csv_jet2_eta", &csv_jet2_eta, "csv_jet2_eta/F");
  t->Branch("csv_jet2_m", &csv_jet2_m, "csv_jet2_m/F");
  t->Branch("csv_jet2_maxtrk", &csv_jet2_maxtrk, "csv_jet2_maxtrk/F");
  t->Branch("csv_jet2_nhf", &csv_jet2_nhf, "csv_jet2_nhf/F");
  t->Branch("csv_jet2_nlep", &csv_jet2_nlep, "csv_jet2_nlep/I");
  t->Branch("csv_jet2_phi", &csv_jet2_phi, "csv_jet2_phi/F");
  t->Branch("csv_jet2_pt", &csv_jet2_pt, "csv_jet2_pt/F");
  t->Branch("csv_jet2_vtx_3Derr", &csv_jet2_vtx_3Derr, "csv_jet2_vtx_3Derr/F");
  t->Branch("csv_jet2_vtx_3Dval", &csv_jet2_vtx_3Dval, "csv_jet2_vtx_3Dval/F");
  t->Branch("csv_jet2_vtx_m", &csv_jet2_vtx_m, "csv_jet2_vtx_m/F");
  t->Branch("csv_jet2_vtx_ntrk", &csv_jet2_vtx_ntrk, "csv_jet2_vtx_ntrk/F");
  t->Branch("csv_jet2_vtx_pt", &csv_jet2_vtx_pt, "csv_jet2_vtx_pt/F");
  t->Branch("event_num", &event_num, "event_num/l");
  t->Branch("jet1_chf", &jet1_chf, "jet1_chf/F");
  t->Branch("jet1_cmva", &jet1_cmva, "jet1_cmva/F");
  t->Branch("jet1_csv", &jet1_csv, "jet1_csv/F");
  t->Branch("jet1_emfrac", &jet1_emfrac, "jet1_emfrac/F");
  t->Branch("jet1_eta", &jet1_eta, "jet1_eta/F");
  t->Branch("jet1_m", &jet1_m, "jet1_m/F");
  t->Branch("jet1_nhf", &jet1_nhf, "jet1_nhf/F");
  t->Branch("jet1_phi", &jet1_phi, "jet1_phi/F");
  t->Branch("jet1_pt", &jet1_pt, "jet1_pt/F");
  t->Branch("jet2_chf", &jet2_chf, "jet2_chf/F");
  t->Branch("jet2_cmva", &jet2_cmva, "jet2_cmva/F");
  t->Branch("jet2_csv", &jet2_csv, "jet2_csv/F");
  t->Branch("jet2_emfrac", &jet2_emfrac, "jet2_emfrac/F");
  t->Branch("jet2_eta", &jet2_eta, "jet2_eta/F");
  t->Branch("jet2_m", &jet2_m, "jet2_m/F");
  t->Branch("jet2_nhf", &jet2_nhf, "jet2_nhf/F");
  t->Branch("jet2_phi", &jet2_phi, "jet2_phi/F");
  t->Branch("jet2_pt", &jet2_pt, "jet2_pt/F");
  t->Branch("lumi_num", &lumi_num, "lumi_num/I");
  t->Branch("mc_weight", &mc_weight, "mc_weight/F");
  t->Branch("met", &met, "met/F");
  t->Branch("met_trigger", &met_trigger, "met_trigger/O");
  t->Branch("metfilter", &metfilter, "metfilter/O");
  t->Branch("metphi", &metphi, "metphi/F");
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
  cmva_hbb_eta = -5;
  cmva_hbb_m = -5;
  cmva_hbb_phi = -5;
  cmva_hbb_pt = -5;
  cmva_jet1 = false;
  cmva_jet1_chf = -5;
  cmva_jet1_cmva = -5;
  cmva_jet1_csv = -5;
  cmva_jet1_emfrac = -5;
  cmva_jet1_eta = -5;
  cmva_jet1_m = -5;
  cmva_jet1_maxtrk = -5;
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
  cmva_jet2_emfrac = -5;
  cmva_jet2_eta = -5;
  cmva_jet2_m = -5;
  cmva_jet2_maxtrk = -5;
  cmva_jet2_nhf = -5;
  cmva_jet2_nlep = 0;
  cmva_jet2_phi = -5;
  cmva_jet2_pt = -5;
  cmva_jet2_vtx_3Derr = -5;
  cmva_jet2_vtx_3Dval = -5;
  cmva_jet2_vtx_m = -5;
  cmva_jet2_vtx_ntrk = -5;
  cmva_jet2_vtx_pt = -5;
  csv_hbb_eta = -5;
  csv_hbb_m = -5;
  csv_hbb_phi = -5;
  csv_hbb_pt = -5;
  csv_jet1 = false;
  csv_jet1_chf = -5;
  csv_jet1_cmva = -5;
  csv_jet1_csv = -5;
  csv_jet1_emfrac = -5;
  csv_jet1_eta = -5;
  csv_jet1_m = -5;
  csv_jet1_maxtrk = -5;
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
  csv_jet2_emfrac = -5;
  csv_jet2_eta = -5;
  csv_jet2_m = -5;
  csv_jet2_maxtrk = -5;
  csv_jet2_nhf = -5;
  csv_jet2_nlep = 0;
  csv_jet2_phi = -5;
  csv_jet2_pt = -5;
  csv_jet2_vtx_3Derr = -5;
  csv_jet2_vtx_3Dval = -5;
  csv_jet2_vtx_m = -5;
  csv_jet2_vtx_ntrk = -5;
  csv_jet2_vtx_pt = -5;
  event_num = event.eventNumber;
  jet1_chf = -5;
  jet1_cmva = -5;
  jet1_csv = -5;
  jet1_emfrac = -5;
  jet1_eta = -5;
  jet1_m = -5;
  jet1_nhf = -5;
  jet1_phi = -5;
  jet1_pt = -5;
  jet2_chf = -5;
  jet2_cmva = -5;
  jet2_csv = -5;
  jet2_emfrac = -5;
  jet2_eta = -5;
  jet2_m = -5;
  jet2_nhf = -5;
  jet2_phi = -5;
  jet2_pt = -5;
  lumi_num = event.lumiNumber;
  mc_weight = 0;
  met = event.pfMet.pt;
  met_trigger = false;
  metfilter = false;
  metphi = event.pfMet.phi;
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

void hbbfile::set_b_jet(const set_b_jet_enum base, const panda::Ref<panda::SecondaryVertex>& vert) {
  set(set_b_jet_names[static_cast<int>(base)] + "", static_cast<bool>(true));
  set(set_b_jet_names[static_cast<int>(base)] + "_vtx_pt", static_cast<float>(vert->pt()));
  set(set_b_jet_names[static_cast<int>(base)] + "_vtx_m", static_cast<float>(vert->m()));
  set(set_b_jet_names[static_cast<int>(base)] + "_vtx_3Dval", static_cast<float>(vert->vtx3DVal));
  set(set_b_jet_names[static_cast<int>(base)] + "_vtx_3Derr", static_cast<float>(vert->vtx3DeVal));
  set(set_b_jet_names[static_cast<int>(base)] + "_vtx_ntrk", static_cast<float>(vert->ntrk));
}

void hbbfile::set_b_leps(const set_b_jet_enum base, const panda::Jet& jet, const int nlep, const panda::Ref<panda::PFCand>& pf) {
  set(set_b_jet_names[static_cast<int>(base)] + "_nlep", static_cast<int>(nlep));
}

void hbbfile::set_b_max_trk(const set_b_jet_enum base, const float maxpt) {
  set(set_b_jet_names[static_cast<int>(base)] + "_maxtrk", static_cast<float>(maxpt));
}

void hbbfile::set_jet(const set_jet_enum base, const panda::Jet& jet) {
  set(set_jet_names[static_cast<int>(base)] + "_pt", static_cast<float>(jet.pt()));
  set(set_jet_names[static_cast<int>(base)] + "_eta", static_cast<float>(jet.eta()));
  set(set_jet_names[static_cast<int>(base)] + "_phi", static_cast<float>(jet.phi()));
  set(set_jet_names[static_cast<int>(base)] + "_m", static_cast<float>(jet.m()));
  set(set_jet_names[static_cast<int>(base)] + "_emfrac", static_cast<float>(jet.cef + jet.nef));
  set(set_jet_names[static_cast<int>(base)] + "_chf", static_cast<float>(jet.chf));
  set(set_jet_names[static_cast<int>(base)] + "_nhf", static_cast<float>(jet.nhf));
  set(set_jet_names[static_cast<int>(base)] + "_csv", static_cast<float>(jet.csv));
  set(set_jet_names[static_cast<int>(base)] + "_cmva", static_cast<float>(jet.cmva));
}

void hbbfile::set_hbb(const set_hbb_enum base, const TLorentzVector& vec) {
  set(set_hbb_names[static_cast<int>(base)] + "_pt", static_cast<float>(vec.Pt()));
  set(set_hbb_names[static_cast<int>(base)] + "_eta", static_cast<float>(vec.Eta()));
  set(set_hbb_names[static_cast<int>(base)] + "_phi", static_cast<float>(vec.Phi()));
  set(set_hbb_names[static_cast<int>(base)] + "_m", static_cast<float>(vec.M()));
}


#endif
