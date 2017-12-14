#ifndef CROMBIE_HBBTREE_H
#define CROMBIE_HBBTREE_H

#include <string>
#include <map>
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "PlotTools/interface/KinematicFunctions.h"
#include "PandaTree/Objects/interface/Event.h"
#include "TLorentzVector.h"

class hbbtree {

 public:
  hbbtree(const char* name, const char* outfile_name);
  ~hbbtree() { write(t); f->Close(); }

  float calomet;
  float calometphi;
  float cmva_hbb_eta;
  float cmva_hbb_m;
  float cmva_hbb_phi;
  float cmva_hbb_pt;
  float cmva_jet1_chf;
  float cmva_jet1_cmva;
  float cmva_jet1_csv;
  float cmva_jet1_emfrac;
  float cmva_jet1_eta;
  float cmva_jet1_m;
  float cmva_jet1_nhf;
  float cmva_jet1_phi;
  float cmva_jet1_pt;
  float cmva_jet1_vtx_3Derr;
  float cmva_jet1_vtx_3Dval;
  float cmva_jet1_vtx_m;
  float cmva_jet1_vtx_ntrk;
  float cmva_jet1_vtx_pt;
  float cmva_jet2_chf;
  float cmva_jet2_cmva;
  float cmva_jet2_csv;
  float cmva_jet2_emfrac;
  float cmva_jet2_eta;
  float cmva_jet2_m;
  float cmva_jet2_nhf;
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
  float csv_jet1_chf;
  float csv_jet1_cmva;
  float csv_jet1_csv;
  float csv_jet1_emfrac;
  float csv_jet1_eta;
  float csv_jet1_m;
  float csv_jet1_nhf;
  float csv_jet1_phi;
  float csv_jet1_pt;
  float csv_jet2_chf;
  float csv_jet2_cmva;
  float csv_jet2_csv;
  float csv_jet2_emfrac;
  float csv_jet2_eta;
  float csv_jet2_m;
  float csv_jet2_nhf;
  float csv_jet2_phi;
  float csv_jet2_pt;
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

 private:
  TFile* f;
  TTree* t;  

};

hbbtree::hbbtree(const char* name, const char* outfile_name) {
  f = new TFile(outfile_name, "RECREATE");
  t = new TTree(name, name);

  t->Branch("calomet", &calomet, "calomet/F");
  t->Branch("calometphi", &calometphi, "calometphi/F");
  t->Branch("cmva_hbb_eta", &cmva_hbb_eta, "cmva_hbb_eta/F");
  t->Branch("cmva_hbb_m", &cmva_hbb_m, "cmva_hbb_m/F");
  t->Branch("cmva_hbb_phi", &cmva_hbb_phi, "cmva_hbb_phi/F");
  t->Branch("cmva_hbb_pt", &cmva_hbb_pt, "cmva_hbb_pt/F");
  t->Branch("cmva_jet1_chf", &cmva_jet1_chf, "cmva_jet1_chf/F");
  t->Branch("cmva_jet1_cmva", &cmva_jet1_cmva, "cmva_jet1_cmva/F");
  t->Branch("cmva_jet1_csv", &cmva_jet1_csv, "cmva_jet1_csv/F");
  t->Branch("cmva_jet1_emfrac", &cmva_jet1_emfrac, "cmva_jet1_emfrac/F");
  t->Branch("cmva_jet1_eta", &cmva_jet1_eta, "cmva_jet1_eta/F");
  t->Branch("cmva_jet1_m", &cmva_jet1_m, "cmva_jet1_m/F");
  t->Branch("cmva_jet1_nhf", &cmva_jet1_nhf, "cmva_jet1_nhf/F");
  t->Branch("cmva_jet1_phi", &cmva_jet1_phi, "cmva_jet1_phi/F");
  t->Branch("cmva_jet1_pt", &cmva_jet1_pt, "cmva_jet1_pt/F");
  t->Branch("cmva_jet1_vtx_3Derr", &cmva_jet1_vtx_3Derr, "cmva_jet1_vtx_3Derr/F");
  t->Branch("cmva_jet1_vtx_3Dval", &cmva_jet1_vtx_3Dval, "cmva_jet1_vtx_3Dval/F");
  t->Branch("cmva_jet1_vtx_m", &cmva_jet1_vtx_m, "cmva_jet1_vtx_m/F");
  t->Branch("cmva_jet1_vtx_ntrk", &cmva_jet1_vtx_ntrk, "cmva_jet1_vtx_ntrk/F");
  t->Branch("cmva_jet1_vtx_pt", &cmva_jet1_vtx_pt, "cmva_jet1_vtx_pt/F");
  t->Branch("cmva_jet2_chf", &cmva_jet2_chf, "cmva_jet2_chf/F");
  t->Branch("cmva_jet2_cmva", &cmva_jet2_cmva, "cmva_jet2_cmva/F");
  t->Branch("cmva_jet2_csv", &cmva_jet2_csv, "cmva_jet2_csv/F");
  t->Branch("cmva_jet2_emfrac", &cmva_jet2_emfrac, "cmva_jet2_emfrac/F");
  t->Branch("cmva_jet2_eta", &cmva_jet2_eta, "cmva_jet2_eta/F");
  t->Branch("cmva_jet2_m", &cmva_jet2_m, "cmva_jet2_m/F");
  t->Branch("cmva_jet2_nhf", &cmva_jet2_nhf, "cmva_jet2_nhf/F");
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
  t->Branch("csv_jet1_chf", &csv_jet1_chf, "csv_jet1_chf/F");
  t->Branch("csv_jet1_cmva", &csv_jet1_cmva, "csv_jet1_cmva/F");
  t->Branch("csv_jet1_csv", &csv_jet1_csv, "csv_jet1_csv/F");
  t->Branch("csv_jet1_emfrac", &csv_jet1_emfrac, "csv_jet1_emfrac/F");
  t->Branch("csv_jet1_eta", &csv_jet1_eta, "csv_jet1_eta/F");
  t->Branch("csv_jet1_m", &csv_jet1_m, "csv_jet1_m/F");
  t->Branch("csv_jet1_nhf", &csv_jet1_nhf, "csv_jet1_nhf/F");
  t->Branch("csv_jet1_phi", &csv_jet1_phi, "csv_jet1_phi/F");
  t->Branch("csv_jet1_pt", &csv_jet1_pt, "csv_jet1_pt/F");
  t->Branch("csv_jet2_chf", &csv_jet2_chf, "csv_jet2_chf/F");
  t->Branch("csv_jet2_cmva", &csv_jet2_cmva, "csv_jet2_cmva/F");
  t->Branch("csv_jet2_csv", &csv_jet2_csv, "csv_jet2_csv/F");
  t->Branch("csv_jet2_emfrac", &csv_jet2_emfrac, "csv_jet2_emfrac/F");
  t->Branch("csv_jet2_eta", &csv_jet2_eta, "csv_jet2_eta/F");
  t->Branch("csv_jet2_m", &csv_jet2_m, "csv_jet2_m/F");
  t->Branch("csv_jet2_nhf", &csv_jet2_nhf, "csv_jet2_nhf/F");
  t->Branch("csv_jet2_phi", &csv_jet2_phi, "csv_jet2_phi/F");
  t->Branch("csv_jet2_pt", &csv_jet2_pt, "csv_jet2_pt/F");
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


#endif
